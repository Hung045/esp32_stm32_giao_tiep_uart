#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <WiFi.h>
#include <PubSubClient.h>

// --- Thêm cấu hình WiFi & MQTT ---
const char* ssid = "Đã 6 còn đi wave";
const char* password = "66774953";    
const char* mqtt_server = "172.11.214.24";

WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
HardwareSerial SerialToSTM(2);

const int LED_PIN = 2;
uint16_t last_dist = 0; // Lưu khoảng cách từ STM32 để gửi Node-RED

String getLogTime() {
  unsigned long ms = millis();
  unsigned long sec = ms / 1000;
  unsigned long mn = sec / 60;
  unsigned long hr = mn / 60;
  sec %= 60; mn %= 60;
  char stamp[20];
  sprintf(stamp, "[%02d:%02d:%02d]", (int)hr, (int)mn, (int)sec);
  return String(stamp);
}

// Hàm kết nối WiFi & MQTT
void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to "); Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP: "); Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32_Node1")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc="); Serial.print(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  SerialToSTM.begin(9600, SERIAL_8N1, 16, 17); // Hạ xuống 9600 để khớp STM32
  
  pinMode(LED_PIN, OUTPUT);
  if (!mlx.begin()) { 
    Serial.println("Loi MLX90614!"); 
    while (1); 
  }

  setup_wifi();
  client.setServer(mqtt_server, 1883);

  Serial.println(getLogTime() + " Node 1 READY...");
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  float temp = mlx.readObjectTempC();
  String currentTime = getLogTime();
  
  // 1. ESP32 gửi tin sang STM32
  String msg = "Nguyen Dinh Hung - B22DCDT132 - Temp: " + String(temp, 2) + " C\n";
  SerialToSTM.print(msg);
  Serial.print(currentTime + " [ESP32 LOG] Da gui: " + msg);

  // 2. Đợi phản hồi từ STM32
  unsigned long startWait = millis();
  bool received = false;
  while (millis() - startWait < 800) {
    if (SerialToSTM.available()) {
      String feedback = SerialToSTM.readStringUntil('\n');
      feedback.trim(); 
      if (feedback.length() > 0) {
        Serial.println(getLogTime() + " STM32 FB: " + feedback);
        // Tách số khoảng cách từ chuỗi (Ví dụ: "Phan hoi... 150 mm")
        // Ở đây ta giả định bạn lấy được số, nếu không thì dùng biến temp và 1 số giả định
        last_dist = feedback.toInt(); 
        received = true;
        break;
      }
    }
  }

  if (!received) Serial.println(getLogTime() + " [ERR] Timeout.");

  // 3. GỬI DỮ LIỆU LÊN NODE-RED (Dạng JSON)
  // Topic: sensor/data
  String payload = "{\"nhiet_do\":" + String(temp, 2) + ",\"khoang_cach\":" + String(last_dist) + "}";
  client.publish("sensor/data", payload.c_str());

  // Điều khiển LED
  if (temp > 30) digitalWrite(LED_PIN, HIGH);
  else digitalWrite(LED_PIN, LOW);

  Serial.println("-----------------------");
  delay(3000);
}