#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

/*
  === HƯỚNG DẪN ĐẤU DÂY (WIRING) ===
  Màn hình OLED I2C:
  - VCC  -> 5V hoặc 3.3V của Arduino Uno
  - GND  -> GND của Arduino Uno
  - SCL  -> Chân A5
  - SDA  -> Chân A4
*/

// ============================================================================
// CHỌN DRIVER CHO MÀN HÌNH CỦA BẠN (Dùng chế độ Page Buffer '_1_' để tiết kiệm RAM cho Uno)
// ============================================================================

// LỰA CHỌN 1: Màn hình SSD1306 (0.96 inch) - Chế độ Page Buffer (tiết kiệm RAM)
// U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// LỰA CHỌN 2: Màn hình SH1106 (1.3 inch) - Chế độ Page Buffer (tiết kiệm RAM)
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Hàm quét thiết bị I2C để kiểm tra phần cứng
void scanI2C() {
  byte error, address;
  int nDevices;

  Serial.println(F("\n--- DANG QUET THIET BI I2C ---"));
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print(F("Da tim thay thiet bi I2C tai dia chi: 0x"));
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
      nDevices++;
    }
    else if (error == 4) {
      Serial.print(F("Loi khong xac dinh tai dia chi: 0x"));
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println(F("KHONG TIM THAY THIET BI I2C NAO!\n"));
    Serial.println(F("-> Vui long kiem tra lai day noi (SDA -> A4, SCL -> A5)."));
  } else {
    Serial.println(F("Quet I2C hoan tat.\n"));
  }
}

void setup() {
  Serial.begin(9600); // Hạ baudrate xuống 9600 để Uno hoạt động ổn định hơn
  while(!Serial) { delay(10); } 
  
  // Khởi tạo thư viện I2C
  Wire.begin();
  
  // Quét I2C trước để debug
  scanI2C();
  
  // Khởi động màn hình OLED
  Serial.println(F("Dang khoi tao OLED..."));
  u8g2.begin();
}

void loop() {
  // --- TRANG 1: HIỂN THỊ CHỮ (Sử dụng Picture Loop bắt buộc cho chế độ Page Buffer '_1_') ---
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB08_tr); 
    u8g2.drawStr(0, 18, "Arduino Uno Test");
    u8g2.drawStr(0, 38, "SH1106 OLED 1.3");
    u8g2.drawFrame(0, 0, 128, 64);
  } while ( u8g2.nextPage() );
  
  delay(3000);

  // --- TRANG 2: VẼ HÌNH HỌC ---
  u8g2.firstPage();
  do {
    u8g2.drawCircle(64, 32, 20, U8G2_DRAW_ALL);
    u8g2.drawDisc(64, 32, 8, U8G2_DRAW_ALL);
    u8g2.drawLine(0, 0, 30, 30);
    u8g2.drawLine(128, 0, 98, 30);
    u8g2.drawLine(0, 64, 30, 34);
    u8g2.drawLine(128, 64, 98, 34);
    u8g2.drawFrame(0, 0, 128, 64);
  } while ( u8g2.nextPage() );

  delay(3000);
}
