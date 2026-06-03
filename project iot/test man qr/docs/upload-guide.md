# 🔧 Hướng dẫn nạp firmware ESP32

## Board ESP32-D0WD-V3 (CH340)

### ⚠️ Chân AR (GPIO0) trên board

Trên board ESP32 này, **GPIO0 được ghi là "AR"** trên pin header (không phải "GP0" hay "D0").

```
Vị trí chân trên pin header:

     ┌───────────────────────┐
     │        ESP32          │
     │                       │
  GND │ ● ● │ 3V3
   AR │ ● ● │ VP/D36        ← AR = GPIO0 (dùng để vào download mode)
  VN/ │ ● ● │ VN/D39
 D34 │ ● ● │ D35
  ... │       │ ...
     │     USB (CH340)      │
     │                       │
     └───────────────────────┘
```

### Tại sao phải nối AR → GND?

Board này dùng chip **CH340** làm USB-to-Serial. Mạch auto-reset trên board **chỉ điều khiển chân EN (Reset)**, **không điều khiển chân GPIO0 (AR)**.

→ esptool không tự đưa chip vào download mode được.
→ Phải nối AR → GND bằng tay để kéo GPIO0 xuống LOW.

---

## Quy trình nạp chi tiết

### Cần có:
- 1 sợi dây jumper (hoặc dây điện nhỏ)
- ESP32 đã cắm USB vào máy

### Các bước:

```
┌─────────────────────────────────────────────────┐
│  BƯỚC 1: Nối dây AR → GND                       │
│                                                  │
│  Tìm chân "AR" trên pin header (bên trái)        │
│  Tìm chân "GND" (bên trái, gần AR)              │
│  Nối 2 chân bằng dây jumper                      │
└──────────────────────┬──────────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────────┐
│  BƯỚC 2: Nhấn RESET                             │
│                                                  │
│  Nhấn và thả nút RESET trên board               │
│  Chip sẽ vào DOWNLOAD_BOOT mode                  │
│  (LED trên board có thể nháy khác bình thường)   │
└──────────────────────┬──────────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────────┐
│  BƯỚC 3: Chạy lệnh nạp                          │
│                                                  │
│  PlatformIO:                                     │
│    pio run -e esp32dev --target upload           │
│                                                  │
│  Hoặc esptool trực tiếp:                         │
│    python -m esptool --chip esp32 --port COM6    │
│      --baud 115200 write_flash -z 0x10000        │
│      .pio/build/esp32dev/firmware.bin            │
│                                                  │
│  Hoặc bấm nút → Upload trên PlatformIO toolbar  │
└──────────────────────┬──────────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────────┐
│  BƯỚC 4: Chờ nạp xong                           │
│                                                  │
│  Thấy:                                           │
│    Writing at 0x00010000... (3%)                  │
│    Writing at 0x000xxxxx... (xx%)                 │
│    ...                                           │
│    Writing at 0x000xxxxx... (100%)                │
│    Hash of data verified.    ← NẠP XONG!         │
└──────────────────────┬──────────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────────┐
│  BƯỚC 5: THÁO dây AR khỏi GND                   │
│                                                  │
│  ⚠️ QUAN TRỌNG: Phải tháo dây ra!               │
│  Nếu không tháo, chip sẽ vẫn kẹt ở download mode │
└──────────────────────┬──────────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────────┐
│  BƯỚC 6: Nhấn RESET                             │
│                                                  │
│  Nhấn và thả nút RESET lần nữa                   │
│  Firmware mới sẽ chạy!                           │
│  Mở Serial Monitor để kiểm tra:                  │
│    pio device monitor -p COM6 -b 115200          │
└─────────────────────────────────────────────────┘
```

---

## Xác nhận firmware chạy

Mở Serial Monitor, sẽ thấy:

```
========================================
  SMART SCALE + VIETQR PAYMENT SYSTEM
  ESP32 DevKit
========================================

[MAIN] Initializing modules...
[OLED] Initializing SH1106...
[OLED] Ready!
[AUDIO] Initializing I2S MAX98357A...
[AUDIO] Ready!
[SCALE] Initializing HX711...
[SCALE] Auto-tare...
[SCALE] Ready!
[KEYPAD] Initializing 4x4 keypad...
[KEYPAD] Ready!
[LCD] Initializing LCD1602 I2C...
[LCD] Ready!
[WIFI] Starting AP: SmartScale_VietQR
[WIFI] AP started! IP: 192.168.4.1
[WEB] Starting web server on port 80
[WEB] Ready! http://192.168.4.1

[MAIN] === SYSTEM READY ===
```

---

## Khắc phục sự cố nạp

| Vấn đề | Nguyên nhân | Giải pháp |
|---------|-------------|-----------|
| `Wrong boot mode detected (0x13)` | AR chưa nối GND | Nối AR → GND, nhấn RESET |
| `No serial data received` | AR nối nhưng chip chưa reset | Nhấn RESET sau khi nối AR |
| Kẹt `waiting for download` sau nạp | AR vẫn nối GND | Tháo dây AR, nhấn RESET |
| `Failed to connect` | Sai COM port | Kiểm tra `pio device list` |
| Noise / corruption | Baud rate quá cao | Dùng `--baud 115200` |
