# คู่มือผู้สอน: ตั้งค่า Telegram Bot และเปิด AI บน Google Colab

ผู้สอนทำหน้าที่เปิด **สมองกล AI (Gemma)** และ **Telegram Bot** บน Google Colab (ฟรี ไม่ต้องติดตั้งโปรแกรมลงเครื่องคอมพิวเตอร์) เพื่อเป็นตัวกลางเชื่อมระหว่างแชตของผู้เรียนกับบอร์ด Cucumber

---

## ขั้นที่ 1: สร้าง Telegram Bot Token
1. เปิดแอป Telegram ค้นหาบัญชี **`@BotFather`** (มีเครื่องหมายติ๊กถูกสีฟ้า)
2. พิมพ์คำสั่ง `/newbot`
3. ตั้งชื่อบอท (Display Name) และตั้ง Username โดย**ต้องลงท้ายด้วย `bot`** (เช่น `cucumber_aiot_bot`)
4. คัดลอก **HTTP API Token** (รูปแบบ `123456789:AAHdqTcvCH1vGWJxf...`) เก็บไว้

---

## ขั้นที่ 2: เปิดไฟล์ Notebook บน Google Colab
1. ดาวน์โหลดไฟล์ `ai/cucumber_aiot_bot.ipynb` จาก Repository นี้ แล้วอัปโหลดเข้า [Google Colab](https://colab.research.google.com/)
2. **เปิดใช้งาน GPU ฟรี (สำคัญมาก):**
   * ไปที่เมนูด้านบน **Runtime > Change runtime type**
   * เลือก Hardware accelerator เป็น **T4 GPU** แล้วกด **Save**

---

## ขั้นที่ 3: รันระบบ AI และเชื่อมต่อบอร์ด
1. กดปุ่ม **Run (▶)** ที่ **Cell ที่ 1** เพื่อติดตั้ง `zstd`, โปรแกรม **Ollama** และดาวน์โหลดโมเดล **`gemma2:2b`** (ใช้เวลาประมาณ 1–2 นาที)
2. ใน **Cell ที่ 2** ให้แก้ไขค่า 2 บรรทัดนี้ให้ตรงกับระบบของคุณ:
   ```python
   TELEGRAM_TOKEN = "วาง_TOKEN_ที่ได้จาก_BotFather"
   BOARD_ID       = "cucumber-room-01"  # ต้องตรงกับ BOARD_ID ในบอร์ด
   ```
3. กดปุ่ม **Run (▶)** ที่ **Cell ที่ 2** เมื่อขึ้นข้อความ:
   ```text
   [MQTT] Connected! Subscribing to cucumber-aiot/cucumber-room-01/telemetry
   === Telegram Bot & AI พร้อมทำงานแล้ว! ลองทักแชตใน Telegram ได้เลย ===
   ```
   แสดงว่าระบบพร้อมให้นักเรียนทักแชตเข้ามาสั่งงานบอร์ดได้ทันที

---

## การปรับแต่ง System Prompt (สำหรับสอนเรื่อง AI Engineering)
ผู้สอนสามารถให้ผู้เรียนทดลองแก้ไขตัวแปร `system_prompt` ใน Cell ที่ 2 เพื่อดูผลลัพธ์ที่เปลี่ยนไป เช่น:
* เปลี่ยนบุคลิกการตอบของบอท (เช่น ตอบแบบนักวิทยาศาสตร์, ตอบแบบหุ่นยนต์)
* เพิ่มเกณฑ์การให้คำแนะนำสุขภาพ เช่น หากความชื้นสัมพัทธ์ต่ำกว่า 40% หรือสูงกว่า 70% ให้บอทเตือนเรื่องความชื้นในห้อง

---

[กลับหน้าแรก](./index.md) | [คู่มือแก้ปัญหาที่พบบ่อย](./04_troubleshooting.md)
