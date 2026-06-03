#include <Arduino.h>

/*
  PlatformIO Test Code for Arduino Uno
  Nhấp nháy đèn LED tích hợp (LED_BUILTIN - chân 13) và gửi tín hiệu qua Serial Monitor.
*/

void setup() {
  // Cấu hình chân LED_BUILTIN làm cổng xuất tín hiệu (OUTPUT)
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Khởi động giao tiếp Serial với tốc độ baud 9600
  Serial.begin(9600);
  Serial.println("Arduino Uno (PlatformIO) đã kết nối thành công!");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // Bật LED
  Serial.println("LED: BAT");
  delay(1000);                       // Chờ 1 giây
  
  digitalWrite(LED_BUILTIN, LOW);    // Tắt LED
  Serial.println("LED: TAT");
  delay(1000);                       // Chờ 1 giây
}
