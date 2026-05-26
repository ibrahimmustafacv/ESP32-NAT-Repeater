/*
 * ╔══════════════════════════════════════════════════════════╗
 * ║         ESP32 WiFi Repeater — Ibrahim Esp (NAT)         ║
 * ║  عرض جميع الشبكات، اختيار أي شبكة، بث تلقائي للإنترنت ║
 * ╚══════════════════════════════════════════════════════════╝
 */

#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <esp_wifi.h>
#include <esp_netif.h>

#if __has_include("lwip/napt.h")
  #include "lwip/napt.h"
  #include "lwip/dns.h"
  #define HAS_NAPT 1
#else
  #define HAS_NAPT 0
#endif

#define AP_SSID   "Ibrahim Esp"
#define AP_PASS   "48522844"

const IPAddress AP_IP(192, 168, 4, 1);
const IPAddress AP_GW(192, 168, 4, 1);
const IPAddress AP_SN(255, 255, 255, 0);

WebServer server(80);
Preferences prefs;

String savedSSID = "";
String savedPass = "";
bool staConnected = false;
bool natActive = false;

// --- استخدم مصفوفات منفصلة بدلاً من struct لتجنب أي خطأ ---
const int MAX_NETWORKS = 30;
String networkSSID[MAX_NETWORKS];
int networkRSSI[MAX_NETWORKS];
bool networkEncrypted[MAX_NETWORKS];
int networkCount = 0;

// ========== NAT (مشاركة الإنترنت) ==========
void enableNAT() {
  natActive = false;
  delay(500);
#if defined(ESP_IDF_VERSION_MAJOR) && (ESP_IDF_VERSION_MAJOR >= 5)
  esp_netif_t* apNetif = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");
  if (apNetif && esp_netif_napt_enable(apNetif) == ESP_OK) natActive = true;
#endif
#if HAS_NAPT
  if (!natActive) {
    ip_napt_enable((uint32_t)AP_IP, 1);
    dns_setserver(0, WiFi.dnsIP());
    natActive = true;
  }
#endif
  if (natActive) Serial.println("[NAT] ✅ مفعّل");
  else Serial.println("[NAT] ❌ غير مفعّل (حزمة ESP32 قديمة؟)");
}

// ========== الاتصال بشبكة مختارة ==========
bool connectToWiFi(String ssid, String pass) {
  WiFi.disconnect(false);      // لا نوقف الـ AP
  delay(200);
  if (pass.length() == 0) WiFi.begin(ssid.c_str());
  else WiFi.begin(ssid.c_str(), pass.c_str());

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 25) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  Serial.println();
  return (WiFi.status() == WL_CONNECTED);
}

// ========== مسح الشبكات ==========
void scanNetworks() {
  WiFi.scanDelete();
  int n = WiFi.scanNetworks(false, false);
  networkCount = (n > MAX_NETWORKS) ? MAX_NETWORKS : n;
  for (int i = 0; i < networkCount; i++) {
    networkSSID[i] = WiFi.SSID(i);
    networkRSSI[i] = WiFi.RSSI(i);
    networkEncrypted[i] = (WiFi.encryptionType(i) != WIFI_AUTH_OPEN);
  }
  Serial.printf("تم العثور على %d شبكة\n", networkCount);
}

// ========== حفظ الإعدادات ==========
void saveConfig(String ssid, String pass) {
  prefs.begin("wifi", false);
  prefs.putString("ssid", ssid);
  prefs.putString("pass", pass);
  prefs.end();
  savedSSID = ssid;
  savedPass = pass;
}

void loadConfig() {
  prefs.begin("wifi", true);
  savedSSID = prefs.getString("ssid", "");
  savedPass = prefs.getString("pass", "");
  prefs.end();
}

// ========== صفحات HTML ==========
String buildNetworkListHTML() {
  if (networkCount == 0) return "<li class='empty'>لا توجد شبكات — اضغط تحديث</li>";
  String html = "";
  for (int i = 0; i < networkCount; i++) {
    String enc = networkEncrypted[i] ? "🔒" : "🔓";
    int bars = 0;
    if (networkRSSI[i] > -55) bars = 4;
    else if (networkRSSI[i] > -70) bars = 3;
    else if (networkRSSI[i] > -80) bars = 2;
    else if (networkRSSI[i] > -90) bars = 1;
    String barHtml = "";
    for (int b = 1; b <= 4; b++)
      barHtml += "<span class='b" + String(b <= bars ? " a" : "") + "'></span>";
    String escSsid = networkSSID[i];
    escSsid.replace("\"", "&quot;");
    html += "<li onclick='selectNetwork(\"" + escSsid + "\", " + String(networkEncrypted[i] ? "1" : "0") + ")'>";
    html += "<span>" + enc + " <b>" + networkSSID[i] + "</b></span>";
    html += "<span style='display:flex;gap:2px;align-items:flex-end'>" + barHtml + "</span>";
    html += "</li>";
  }
  return html;
}

String mainHTML() {
  bool connected = (WiFi.status() == WL_CONNECTED);
  String natStatus = natActive ? "✅ مفعّل" : (connected ? "⚠️ لم يُفعّل بعد" : "❌ غير مفعّل");

  String html = R"rawliteral(
<!DOCTYPE html>
<html dir="rtl" lang="ar">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Ibrahim Esp — مكرر الواي فاي</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{background:#0d1117;color:#e6edf3;font-family:'Segoe UI',sans-serif;padding:20px}
.container{max-width:650px;margin:auto}
.card{background:#161b22;border:1px solid #30363d;border-radius:16px;padding:20px;margin-bottom:20px}
h1{text-align:center;margin-bottom:20px;font-size:1.6rem;background:linear-gradient(90deg,#58a6ff,#bf8fff);-webkit-background-clip:text;-webkit-text-fill-color:transparent}
.row{display:grid;grid-template-columns:1fr 1fr;gap:12px;margin-top:12px}
.stat{background:#0d1117;border-radius:10px;padding:10px;border:1px solid #30363d}
.sl{font-size:0.7rem;color:#8b949e;margin-bottom:4px}
.sv{font-weight:bold;word-break:break-all}
.badge{display:inline-flex;align-items:center;gap:8px;padding:4px 12px;border-radius:20px;font-size:0.8rem;font-weight:bold;margin-bottom:12px}
.ok{background:#0d2918;color:#56d364;border:1px solid #238636}
.err{background:#2d1117;color:#f85149;border:1px solid #da3633}
.dot{width:8px;height:8px;border-radius:50%}
.dok{background:#56d364}
.derr{background:#f85149}
.btn{width:100%;padding:12px;border:none;border-radius:10px;font-weight:bold;font-size:0.9rem;cursor:pointer;margin-top:8px}
.btn-primary{background:linear-gradient(135deg,#58a6ff,#4059b5);color:#fff}
.btn-secondary{background:#21262d;color:#e6edf3;border:1px solid #30363d}
.btn-danger{background:linear-gradient(135deg,#da3633,#a11d1a);color:#fff}
ul{list-style:none;margin-top:12px;max-height:300px;overflow-y:auto}
li{background:#0d1117;border:1px solid #21262d;border-radius:12px;padding:10px 12px;margin-bottom:8px;display:flex;justify-content:space-between;align-items:center;cursor:pointer}
li:hover{border-color:#58a6ff;background:#161b22}
.selbox{background:#0d2136;border:1px dashed #58a6ff;border-radius:10px;padding:10px;font-weight:bold;color:#58a6ff;margin:10px 0}
input{width:100%;padding:10px;background:#0d1117;border:1px solid #30363d;border-radius:10px;color:#fff;margin-bottom:12px}
.b{width:4px;border-radius:2px;background:#30363d}
.b:nth-child(1){height:5px}.b:nth-child(2){height:9px}
.b:nth-child(3){height:13px}.b:nth-child(4){height:17px}
.b.a{background:#58a6ff}
footer{text-align:center;color:#484f58;font-size:0.7rem;margin-top:15px}
</style>
<script>
let selectedSsid = "";
let selectedEnc = 0;
function selectNetwork(ssid, enc){
  selectedSsid = ssid;
  selectedEnc = enc;
  document.getElementById('selectedBox').innerText = ssid;
  document.getElementById('ssidHidden').value = ssid;
  let pwInput = document.getElementById('password');
  pwInput.value = '';
  pwInput.placeholder = enc ? 'أدخل كلمة المرور' : 'شبكة مفتوحة (اتركه فارغاً)';
  document.querySelectorAll('li').forEach(li => li.classList.remove('sel'));
  event.currentTarget.classList.add('sel');
}
function refreshNetworks(){
  document.getElementById('netList').innerHTML = '<li class="empty">جاري المسح...</li>';
  fetch('/scan').then(r=>r.text()).then(html=>{
    document.getElementById('netList').innerHTML = html;
  });
}
function connectAndRepeater(){
  let ssid = document.getElementById('ssidHidden').value;
  let pass = document.getElementById('password').value;
  if(!ssid){ alert('اختر شبكة أولاً'); return; }
  if(selectedEnc && pass.length<8 && !confirm('كلمة المرور أقل من 8 أحرف، استمر؟')) return;
  if(!confirm('الاتصال بـ "'+ssid+'" ثم إعادة التشغيل والبث؟')) return;
  fetch('/connect', {
    method:'POST',
    headers:{'Content-Type':'application/x-www-form-urlencoded'},
    body:'ssid='+encodeURIComponent(ssid)+'&pass='+encodeURIComponent(pass)
  }).then(()=>{
    document.querySelector('.card:nth-child(2)').innerHTML = '<p style="text-align:center;padding:20px">⏳ جاري الاتصال وإعادة التشغيل...</p>';
  });
}
function resetSettings(){
  if(confirm('مسح جميع الإعدادات وإعادة التشغيل؟')) fetch('/reset',{method:'POST'}).then(()=>setTimeout(()=>location.reload(),5000));
}
</script>
</head>
<body>
<div class="container">
<h1>📡 Ibrahim Esp — مكرر الواي فاي</h1>
<div class="card">
<span class="badge )rawliteral";
  html += (WiFi.status() == WL_CONNECTED) ? "ok\"><span class='dot dok'></span>✅ متصل ومشارك الإنترنت" : "err\"><span class='dot derr'></span>❌ غير متصل";
  html += "</span><div class='row'>";
  html += "<div class='stat'><div class='sl'>الراوتر المتصل</div><div class='sv'>" + String(WiFi.SSID()) + "</div></div>";
  html += "<div class='stat'><div class='sl'>IP المحلي</div><div class='sv'>" + WiFi.localIP().toString() + "</div></div>";
  html += "<div class='stat'><div class='sl'>الشبكة المبثوثة</div><div class='sv'>" AP_SSID "</div></div>";
  html += "<div class='stat'><div class='sl'>حالة NAT</div><div class='sv'>" + natStatus + "</div></div>";
  html += "<div class='stat'><div class='sl'>الأجهزة المتصلة</div><div class='sv'>" + String(WiFi.softAPgetStationNum()) + "</div></div>";
  html += "</div></div>";

  html += R"rawliteral(
<div class="card">
<button class="btn btn-secondary" onclick="refreshNetworks()">🔄 تحديث الشبكات</button>
<ul id="netList">)rawliteral";
  html += buildNetworkListHTML();
  html += R"rawliteral(</ul>
<input type="hidden" id="ssidHidden">
<div class="selbox" id="selectedBox">لم تختر شبكة بعد</div>
<input type="password" id="password" placeholder="كلمة المرور">
<button class="btn btn-primary" onclick="connectAndRepeater()">✅ اتصل وابدأ البث</button>
</div>
<div class="card">
<button class="btn btn-danger" onclick="resetSettings()">🗑️ حذف الإعدادات وإعادة التشغيل</button>
</div>
<footer>ESP32 NAT Repeater | 192.168.4.1</footer>
</div>
</body>
</html>)rawliteral";
  return html;
}

// ========== معالجات الـ WebServer ==========
void handleRoot() { server.send(200, "text/html; charset=utf-8", mainHTML()); }
void handleScan() {
  scanNetworks();
  server.send(200, "text/html; charset=utf-8", buildNetworkListHTML());
}
void handleConnect() {
  if (!server.hasArg("ssid") || server.arg("ssid").isEmpty()) {
    server.send(400, "text/plain", "missing ssid");
    return;
  }
  String ssid = server.arg("ssid");
  String pass = server.arg("pass");
  saveConfig(ssid, pass);
  server.send(200, "text/plain", "OK");
  delay(500);
  ESP.restart();
}
void handleReset() {
  prefs.begin("wifi", false);
  prefs.clear();
  prefs.end();
  server.send(200, "text/plain", "OK");
  delay(500);
  ESP.restart();
}

// ========== الإعداد الرئيسي ==========
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\nبدء تشغيل مكرر Ibrahim Esp");

  loadConfig();

  // إعداد الـ AP (يجب أن يعمل دائماً)
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(AP_IP, AP_GW, AP_SN);
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.printf("AP '%s' يعمل على IP %s\n", AP_SSID, AP_IP.toString().c_str());

  // محاولة الاتصال بشبكة محفوظة
  if (savedSSID.length() > 0) {
    Serial.printf("محاولة الاتصال بالشبكة المحفوظة: %s\n", savedSSID.c_str());
    if (connectToWiFi(savedSSID, savedPass)) {
      staConnected = true;
      Serial.println("اتصال ناجح! تفعيل NAT...");
      enableNAT();
    } else {
      Serial.println("فشل الاتصال بالشبكة المحفوظة");
    }
  }

  // مسح أولي للشبكات
  scanNetworks();

  // إعداد خادم الويب
  server.on("/", handleRoot);
  server.on("/scan", handleScan);
  server.on("/connect", HTTP_POST, handleConnect);
  server.on("/reset", HTTP_POST, handleReset);
  server.begin();
  Serial.println("خادم الويب جاهز على http://192.168.4.1");
}

void loop() {
  server.handleClient();
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 10000) {
    lastCheck = millis();
    if (WiFi.status() == WL_CONNECTED && !natActive) {
      Serial.println("إعادة تفعيل NAT بعد استقرار الاتصال");
      enableNAT();
    }
  }
}