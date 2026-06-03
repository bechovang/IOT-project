#ifndef CONFIG_H
#define CONFIG_H

// ============================================================
//  SMART SCALE + VIETQR - Pin Configuration (ESP32 DevKit)
// ============================================================

// --- I2C Bus (OLED + LCD share same bus) ---
#define I2C_SDA       21
#define I2C_SCL       22

// --- HX711 Load Cell ---
#define HX711_DOUT    25
#define HX711_SCK     26
#define HX711_SCALE_FACTOR  2280.0f   // Calibrate với quả cân chuẩn
#define HX711_OFFSET         0L        // Tare offset

// --- 4x4 Keypad ---
#define KP_ROWS       4
#define KP_COLS       4
#define KP_R1         13
#define KP_R2         12
#define KP_R3         14
#define KP_R4         27
#define KP_C1         32
#define KP_C2         33
#define KP_C3         4
#define KP_C4         5

// --- I2S Audio (MAX98357A) ---
#define I2S_BCLK      16
#define I2S_LRCLK     17
#define I2S_DOUT      2
#define I2S_SAMPLE_RATE   16000
#define I2S_SAMPLE_BITS   16

// --- Status LED ---
#define LED_PIN       2   // Built-in LED trên ESP32 DevKit

// --- OLED SH1106 ---
#define OLED_ADDR     0x3C    // I2C address
#define OLED_WIDTH    128
#define OLED_HEIGHT   64

// --- LCD1602 ---
#define LCD_ADDR      0x27    // I2C address (hoặc 0x3F)
#define LCD_COLS      16
#define LCD_ROWS      2

// --- WiFi AP ---
#define WIFI_SSID     "SmartScale_VietQR"
#define WIFI_PASS     "12345678"
#define WIFI_CHANNEL  1
#define WIFI_MAX_CONN 4

// --- Web Server ---
#define WEB_PORT      80

// --- VietQR ---
#define VIETQR_BANK_CODE    "970436"   // Mã ngân hàng (VD: BIDV)
#define VIETQR_ACCOUNT_NO   "1234567890"  // Số tài khoản
#define VIETQR_ACCOUNT_NAME "NGUYEN VAN A" // Tên chủ TK (viết hoa, không dấu)
#define VIETQR_TEMPLATE     "compact2"     // QR template

// --- Application ---
#define MAX_PRICE_DIGITS   8       // Tối đa 8 số cho đơn giá
#define WEIGHT_STABLE_THRESHOLD 0.5f  // Gram, ngưỡng cân ổn định
#define DISPLAY_UPDATE_MS   500      // Cập nhật display mỗi 500ms
#define BEEP_DURATION_MS    150      // Thời gian beep

#endif // CONFIG_H
