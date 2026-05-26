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

## 🖼️ معاينة سريعة للأداة

<p align="center">
  <img src="./images/dashboard.png" width="45%" alt="واجهة التحكم"/>
  <img src="./images/diagram.png" width="45%" alt="مخطط الاتصال"/>
</p>

<p align="center">
  <sub>الشكل 1: واجهة التحكم الرئيسية (يسار) – الشكل 2: مخطط عمل المكرر (يمين)</sub>
</p>

---

## 📑 فهرس المحتويات
- [✨ الميزات](#-الميزات)
- [🧠 كيف يعمل](#-كيف-يعمل)
- [🛠️ المتطلبات](#️-المتطلبات)
- [📦 تثبيت حزمة ESP32](#-تثبيت-حزمة-esp32)
- [🚀 رفع الكود على ESP32](#-رفع-الكود-على-esp32)
- [🕹️ دليل الاستخدام](#️-دليل-الاستخدام)
- [🔧 إعدادات مخصصة](#-إعدادات-مخصصة)
- [🧪 اختبار الأداة](#-اختبار-الأداة)
- [❓ الأسئلة الشائعة](#-الأسئلة-الشائعة)
- [👤 المطور](#-المطور)
- [📄 الترخيص](#-الترخيص)

---

## ✨ الميزات

| الميزة | الوصف |
|--------|-------|
| 🔄 **مكرر WiFi كامل** | يتصل بأي شبكة موجودة ويعيد بثها مع إنترنت كامل |
| 🌐 **NAT مدمج** | مشاركة الإنترنت من الشبكة الأم إلى الشبكة المبثوثة |
| 🎛️ **واجهة ويب احترافية** | لوحة تحكم رسومية لعرض الشبكات وإدارتها |
| 🕒 **مسح فوري للشبكات** | زر تحديث لمسح جميع الشبكات المتاحة في ثوانٍ |
| 🔒 **اختيار أي شبكة** | قائمة ديناميكية بجميع الشبكات وقوة الإشارة والأمان |
| 💾 **حفظ الإعدادات** | تخزين بيانات الشبكة في الذاكرة الدائمة (Preferences) |
| 🔁 **إعادة تشغيل تلقائي** | بعد حفظ الإعدادات، يعيد ESP32 التشغيل ليطبقها فوراً |
| 🧹 **إعادة ضبط المصنع** | زر واحد لحذف جميع الإعدادات وإعادة التشغيل |
| 📊 **عرض حالة الاتصال** | IP المحلي، قوة الإشارة، عدد الأجهزة المتصلة |
| 🌍 **دعم اللغة العربية** | واجهة ويب كاملة باللغة العربية (مع دعم الإنجليزية) |

---

## 🧠 كيف يعمل؟
