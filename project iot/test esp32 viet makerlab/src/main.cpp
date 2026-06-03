#include <Arduino.h>
#include "WiFi.h"

// Chân LED tích hợp trên hầu hết các board ESP32 là GPIO 2. 
// Nếu board của bạn dùng chân khác, hãy đổi số 2 thành chân tương ứng.
#define LED_PIN 2

void setup() {
  // Cấu hình Serial Monitor với tốc độ 115200
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n====================================");
  Serial.println("   ESP32 MakerLab Diagnostic Test   ");
  Serial.println("====================================");

  // Cấu hình chân LED
  pinMode(LED_PIN, OUTPUT);

  // Thiết lập WiFi ở chế độ Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  Serial.println("Khoi tao he thong hoan tat!");
}

void loop() {
  // 1. Nhấp nháy LED tích hợp
  Serial.println("LED ON");
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  
  Serial.println("LED OFF");
  digitalWrite(LED_PIN, LOW);
  delay(500);

  // 2. Quét mạng WiFi xung quanh để test chip thu phát sóng (RF)
  Serial.println("Bat dau quet WiFi...");
  int n = WiFi.scanNetworks();
  Serial.println("Quet hoan tat!");
  
  if (n == 0) {
    Serial.println("Khong tim thay mang WiFi nao.");
  } else {
    Serial.print(n);
    Serial.println(" mang WiFi tim thay:");
    for (int i = 0; i < n; ++i) {
      // In SSID và RSSI (độ mạnh tín hiệu) của từng mạng tìm thấy
      Serial.printf("%d: %s (%d dBm)\n", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
      delay(10);
    }
  }
  Serial.println("------------------------------------");
  
  // Đợi 5 giây trước khi thực hiện lượt quét/nháy LED tiếp theo
  delay(5000);
}
