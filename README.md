# Cucumber AIoT for Beginners

[![Open Documentation](https://img.shields.io/badge/📖_เข้าสู่เว็บไซต์บทเรียน-GitHub_Pages-2ea44f?style=for-the-badge)](https://nataph48.github.io/cucumber-aiot-for-beginners/)

ชุดสื่อการเรียนรู้การสร้างระบบ **AIoT (Artificial Intelligence of Things)** ฉบับเริ่มต้น ด้วยการสร้าง **"ผู้ช่วยดูแลสภาพแวดล้อมในห้องอัจฉริยะ (Smart Room Assistant)"** โดยใช้บอร์ด **Cucumber (ESP32-S2)** ทำงานร่วมกับโมเดล **AI (Gemma 2)** บน Google Colab และสั่งงานด้วยภาษาไทยผ่าน **Telegram Bot**

> **เว็บไซต์คู่มือการสอนฉบับเต็ม:** [https://nataph48.github.io/cucumber-aiot-for-beginners/](https://nataph48.github.io/cucumber-aiot-for-beginners/)

---

## ภาพรวมการทำงานของระบบ (System Architecture)

```text
+-----------------------------------+         +-----------------------+
|   บอร์ด Cucumber (ESP32-S2)        |         |     MQTT Broker       |
|  - อ่านค่า Temp / Humidity / Press | ------> |  (broker.emqx.io)     |
|  - ควบคุมไฟ RGB LED (Auto/Manual) | <------ |  รับ-ส่งข้อมูล JSON     |
+-----------------------------------+         +-----------------------+
                                                         ^  |
                                           ส่งคำสั่ง JSON  |  | ส่งค่าเซนเซอร์
                                                         |  v
+-----------------------------------+         +-----------------------+
|          ผู้เรียน (Telegram)        | <-----> |  Google Colab (ผู้สอน) |
| พิมพ์แชตภาษาไทยถาม-ตอบและสั่งเปิดไฟ   |         |  AI (Gemma 2) + Bot   |
+-----------------------------------+         +-----------------------+
```

### จุดเด่นของชุดการเรียนรู้นี้
* **ไม่ต้องต่อวงจรเพิ่ม:** ใช้เซนเซอร์ **HTS221** (อุณหภูมิ/ความชื้น), **BMP280** (ความดันอากาศ) และไฟ **NeoPixel RGB LED** ที่ฝังอยู่บนบอร์ด Cucumber ทั้งหมด
* **ไม่ต้องติดตั้ง AI ลงเครื่อง:** รันโมเดล **Gemma 2 (2B)** ผ่าน Ollama บน **Google Colab (T4 GPU)** ฟรี
* **สั่งงานด้วยภาษาธรรมชาติ:** พิมพ์คุยภาษาไทยใน Telegram เพื่อถามสภาพอากาศในห้องหรือสั่งเปลี่ยนสีไฟได้ทันที โดยแยกส่วนตรรกะความปลอดภัย (`AUTO` / `MANUAL`) ไว้ที่ตัวบอร์ด

---

## โครงสร้างไฟล์ใน Repository

```text
cucumber-aiot-for-beginners/
├── ai/
│   └── cucumber_aiot_bot.ipynb     # โค้ด Google Colab สำหรับรัน AI (Gemma) และ Telegram Bot
├── docs/                           # ต้นฉบับหน้าเว็บไซต์บทเรียน (GitHub Pages)
│   ├── _config.yml                 # ตั้งค่าธีมและปลั๊กอินลิงก์ของ Jekyll
│   ├── index.md                    # หน้าแรกของเว็บไซต์
│   ├── 01_student_guide.md         # บทที่ 1: เตรียมบอร์ด Cucumber และอัปโหลดโค้ด
│   ├── 02_student_chat.md          # บทที่ 2: สั่งงานผ่านแชต Telegram และเข้าใจ AI
│   ├── 03_instructor_guide.md      # คู่มือผู้สอน: ตั้งค่า Bot และรันระบบบน Colab
│   └── 04_troubleshooting.md       # คู่มือแก้ปัญหาที่พบบ่อย
├── firmware/
│   ├── src/
│   │   └── main.cpp                # โค้ดบอร์ด Cucumber (C++ / Arduino Framework)
│   └── platformio.ini              # ตั้งค่าไลบรารีและพอร์ต Serial 115200
└── README.md
```

---

## เริ่มต้นใช้งานอย่างรวดเร็ว (Quick Start)

1. **อ่านคู่มือและบทเรียนทั้งหมดได้ที่:** [หน้าเว็บไซต์ GitHub Pages](https://nataph48.github.io/cucumber-aiot-for-beginners/)
2. **ฝั่งบอร์ด (Firmware):** เปิดโฟลเดอร์ `firmware/` ด้วย VS Code (PlatformIO) แก้ไขชื่อ Wi-Fi และ `BOARD_ID` ใน `src/main.cpp` แล้วอัปโหลดลงบอร์ด Cucumber
3. **ฝั่ง AI และบอท (Colab):** เปิดไฟล์ `ai/cucumber_aiot_bot.ipynb` บน Google Colab เลือก Runtime เป็น **T4 GPU** ใส่ `TELEGRAM_TOKEN` และกดรันทุก Cell

---

## แหล่งอ้างอิงและแรงบันดาลใจ
* โครงสร้างบทเรียนและแนวคิดการแยกส่วน **AI (แปลภาษา)** ออกจาก **Rule-Based Control (กฎควบคุมอุปกรณ์)** ดัดแปลงมาจากโปรเจกต์ [AIoT for Beginners](https://vsupacha.github.io/aiot-for-beginners/)
