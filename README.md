![Logo](./Logo.png)

<p align="center">
  <img src="https://readme-typing-svg.demolab.com?font=Fira+Code&weight=600&size=28&duration=3000&pause=500&color=58A6FF&center=true&vCenter=true&random=false&width=500&lines=ESP32+NAT+Repeater;Ibrahim+Esp+Repeater" alt="Typing SVG" />
</p>

<p align="center">
  <img src="https://img.shields.io/badge/ESP32-2C3E50?style=for-the-badge&logo=espressif&logoColor=white"/>
  <img src="https://img.shields.io/badge/Arduino_IDE-00979D?style=for-the-badge&logo=arduino&logoColor=white"/>
  <img src="https://img.shields.io/badge/NAT_Enabled-2E8B57?style=for-the-badge&logo=network&logoColor=white"/>
  <img src="https://img.shields.io/badge/License-MIT-blue?style=for-the-badge"/>
</p>

<p align="center">
  <b>📡 حول أي شبكة WiFi إلى مكرر (Repeater) مع NAT كامل – لوحة تحكم ويب عربية/إنجليزية</b><br>
  <sub>قم بتوسيع نطاق شبكتك اللاسلكية بسهولة باستخدام ESP32</sub>
</p>

---

## ✨ الميزات السريعة

| الميزة | الوصف |
|--------|-------|
| 🔄 **مكرر WiFi كامل** | يتصل بأي شبكة موجودة ويعيد بثها مع إنترنت كامل |
| 🌐 **NAT مدمج** | مشاركة الإنترنت من الشبكة الأم إلى الشبكة المبثوثة |
| 🎛️ **واجهة ويب احترافية** | لوحة تحكم رسومية لعرض الشبكات وإدارتها |
| 💾 **حفظ الإعدادات** | تخزين بيانات الشبكة في الذاكرة الدائمة |
| 🔁 **إعادة تشغيل تلقائي** | بعد حفظ الإعدادات، يعيد ESP32 التشغيل ليطبقها فوراً |
| 🧹 **إعادة ضبط المصنع** | زر واحد لحذف جميع الإعدادات |
| 🌍 **دعم اللغة العربية** | واجهة ويب كاملة باللغة العربية |

---

## 🖼️ معاينة سريعة للأداة

<p align="center">
  <img src="./images/dashboard.png" width="45%" alt="واجهة التحكم الرئيسية"/>
  <img src="./images/diagram.png" width="45%" alt="مخطط عمل المكرر"/>
</p>

<p align="center">
  <sub>الشكل 1: واجهة التحكم على 192.168.4.1 – الشكل 2: مخطط الاتصال (راوتر → ESP32 → شبكة Ibrahim Esp)</sub>
</p>

---

## 🚀 روابط سريعة

| الملف | المحتوى |
|-------|----------|
| [📘 التثبيت والإعداد](INSTALL.md) | شرح تثبيت حزمة ESP32 ورفع الكود على اللوحة |
| [🕹️ دليل الاستخدام](USAGE.md) | شرح كيفية استخدام الواجهة وتغيير الشبكات |
| [👤 المطور والرخصة](CREDITS.md) | معلومات المطور والترخيص والمساهمين |

---

## ⚡ تشغيل سريع (للخبراء)

```bash
git clone https://github.com/ibrahimmustafacv/ESP32-NAT-Repeater.git
cd ESP32-NAT-Repeater
# افتح الملف .ino في Arduino IDE → ارفعه على ESP32
# بعد الرفع: اتصل بشبكة "Ibrahim Esp" (48522844) وافتح http://192.168.4.1