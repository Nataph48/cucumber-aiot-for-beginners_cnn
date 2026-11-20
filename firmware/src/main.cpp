#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HTS221.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_NeoPixel.h>

// ================= 1. ตั้งค่า Wi-Fi และรหัสบอร์ด =================
const char* WIFI_SSID = "YOUR_WIFI_SSID";       // ใส่ชื่อ Wi-Fi 2.4GHz ของคุณ
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";   // ใส่รหัสผ่าน Wi-Fi

// ตั้งชื่อเฉพาะตัว (ห้ามซ้ำกับคนอื่นบน Public Broker)
const char* BOARD_ID  = "cucumber-room-01"; 

const char* MQTT_BROKER = "broker.emqx.io";
const int   MQTT_PORT   = 1883;

String topicTelemetry = String("cucumber-aiot/") + BOARD_ID + "/telemetry";
String topicCommand   = String("cucumber-aiot/") + BOARD_ID + "/command";

// ================= 2. ตั้งค่าขาอุปกรณ์บนบอร์ด Cucumber =================
#define I2C_SDA_PIN   41
#define I2C_SCL_PIN   40
#define RGB_LED_PIN   18
#define NUM_PIXELS    1

Adafruit_HTS221 hts;
Adafruit_BMP280 bmp;
Adafruit_NeoPixel pixel(NUM_PIXELS, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);

WiFiClient espClient;
PubSubClient mqtt(espClient);

// ================= 3. ตัวแปรสถานะของระบบ =================
String currentMode = "AUTO";   // "AUTO" หรือ "MANUAL"
String currentLed  = "GREEN";  // "RED", "GREEN", "BLUE", "OFF"
float  tempThreshold = 28.0;   // เกณฑ์อุณหภูมิสำหรับโหมด AUTO
unsigned long lastPublish = 0;

void setLedColor(String color) {
  currentLed = color;
  if (color == "RED") {
    pixel.setPixelColor(0, pixel.Color(255, 0, 0));
  } else if (color == "GREEN") {
    pixel.setPixelColor(0, pixel.Color(0, 255, 0));
  } else if (color == "BLUE") {
    pixel.setPixelColor(0, pixel.Color(0, 100, 255));
  } else {
    currentLed = "OFF";
    pixel.setPixelColor(0, pixel.Color(0, 0, 0));
  }
  pixel.show();
}

void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, payload, length);
  if (error) {
    Serial.println("[MQTT] JSON parse failed!");
    return;
  }

  String cmd = doc["cmd"] | "";
  Serial.println("[MQTT] Received Command: " + cmd);

  if (cmd == "MODE_AUTO") {
    currentMode = "AUTO";
  } else if (cmd == "LED_RED") {
    currentMode = "MANUAL";
    setLedColor("RED");
  } else if (cmd == "LED_GREEN") {
    currentMode = "MANUAL";
    setLedColor("GREEN");
  } else if (cmd == "LED_BLUE") {
    currentMode = "MANUAL";
    setLedColor("BLUE");
  } else if (cmd == "LED_OFF") {
    currentMode = "MANUAL";
    setLedColor("OFF");
  }
}

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("[WiFi] Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n[WiFi] Connected! IP: " + WiFi.localIP().toString());
}

void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("[MQTT] Connecting to broker...");
    String clientId = String(BOARD_ID) + "-" + String(random(1000, 9999));
    if (mqtt.connect(clientId.c_str())) {
      Serial.println("Connected!");
      mqtt.subscribe(topicCommand.c_str());
      Serial.println("[MQTT] Subscribed: " + topicCommand);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" retrying in 3s...");
      delay(3000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {
    delay(10);
  }
  Serial.println("\n=== Cucumber AIoT Booting (115200 baud) ===");

  pixel.begin();
  pixel.setBrightness(40);
  setLedColor("OFF");
  Serial.println("[Init] NeoPixel RGB LED OK");

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  if (!hts.begin_I2C()) {
    Serial.println("[Error] ไม่พบเซนเซอร์ HTS221!");
    while (1) { delay(10); }
  }
  Serial.println("[Init] HTS221 Sensor OK");

  if (!bmp.begin(0x76) && !bmp.begin(0x77)) {
    Serial.println("[Error] ไม่พบเซนเซอร์ BMP280!");
    while (1) { delay(10); }
  }
  Serial.println("[Init] BMP280 Sensor OK");

  connectWiFi();
  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt.setCallback(onMqttMessage);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();

  if (millis() - lastPublish >= 5000) {
    lastPublish = millis();

    sensors_event_t humidity, temp;
    hts.getEvent(&humidity, &temp);
    float t = round(temp.temperature * 10.0) / 10.0;
    float h = round(humidity.relative_humidity * 10.0) / 10.0;
    float p = round((bmp.readPressure() / 100.0F) * 10.0) / 10.0;

    if (currentMode == "AUTO") {
      if (t > tempThreshold) {
        setLedColor("RED");
      } else {
        setLedColor("GREEN");
      }
    }

    JsonDocument doc;
    doc["temp"]     = t;
    doc["humidity"] = h;
    doc["pressure"] = p;
    doc["mode"]     = currentMode;
    doc["led"]      = currentLed;

    char buffer[160];
    serializeJson(doc, buffer);

    mqtt.publish(topicTelemetry.c_str(), buffer);
    Serial.println("[Telemetry] " + String(buffer));
  }
}
