# Hướng dẫn kiểm tra loa MAX98357A với ESP32 (PlatformIO)

Tài liệu hướng dẫn kết nối, cấu hình và kiểm tra **mạch khuếch đại âm thanh I2S MAX98357A** với **ESP32** qua PlatformIO (VS Code). Code test phát một âm tone liên tục ~1 kHz để xác nhận toàn bộ chuỗi **ESP32 → I2S → MAX98357A → loa** hoạt động.

---

## 1. Mục đích

Đây là bài test "khói" (smoke test): nếu loa kêu **"beeeep"** đều thì chứng tỏ:

- ESP32 chạy ổn.
- Giao tiếp I2S hoạt động.
- Module MAX98357A nhận dữ liệu và khuếch đại được.
- Loa và dây nối đúng.

Sau khi qua bài test này, bạn có thể tiến lên các ứng dụng thực tế (phát MP3 từ thẻ SD, phát giọng nói, cảnh báo âm thanh...).

---

## 2. Cấu trúc dự án

```
test MAX98357A loa/
├─ platformio.ini      Cấu hình PlatformIO (board esp32dev, ghim platform 6.9.0)
├─ src/main.cpp        Code test: phát tone 1kHz qua I2S
├─ nối mạch.txt        Sơ đồ nối dây (bản tóm tắt)
├─ DOC.md              Tài liệu này
└─ .gitignore
```

---

## 3. Phần cứng

### 3.1. ESP32 (board dạng Vietduino)
Board ESP32-WROOM dùng chip ESP32 + mạch nạp CH340 qua USB-C. **Lưu ý quan trọng:** trên board này, **nhãn in "D" trên board KHÔNG bằng số GPIO** (xem mục 5).

### 3.2. Module MAX98357A (khuếch đại I2S)
Module khuếch đại âm thanh класса-D, nhận dữ liệu âm thanh số qua chuẩn **I2S**, xuất ra loa. Công suất tối đa ~3W với loa 4Ω.

Các chân trên module:

| Chân module | Ý nghĩa | ESP32 nối |
|-------------|---------|-----------|
| **VIN** | Nguồn cấp (3.3V–5V) | 3V3 |
| **GND** | Mass | GND |
| **DIN** | Data In — nhận dữ liệu I2S | GPIO25 |
| **BCLK** | Bit Clock — xung clock bit | GPIO26 |
| **LRC** (WS) | Word Select / Left-Right Channel | GPIO27 |
| **GAIN** | Cài độ khuếch đại | GND |
| **SD** | Chọn kênh / chế độ | (để trống) |
| **SPK+ / SPK-** | Xuất ra loa | Loa |

> **Lưu ý DIN vs DOUT:** MAX98357A **chỉ có DIN** (nhận dữ liệu), không có DOUT. Trong code ESP32 người ta hay đặt tên biến là `I2S_DOUT` (data *out* của ESP32) nhưng dây thật nối sang **DIN** của module. Tức là: **ESP32 xuất → MAX98357A nhận**. Đừng tìm chân DOUT trên module.

> **GAIN:** nối GAIN → GND để chọn gain mặc định ~9 dB. Để trống cũng được (module tự kéo về mặc định).

### 3.3. Loa
Loa **4Ω hoặc 8Ω** (loa động lực bình thường). Nối 2 dây vào **SPK+** và **SPK-** (không phân cực, đảo cũng được).

---

## 4. Sơ đồ nối dây

```
ESP32 (board)          MAX98357A        Loa
3V3            ──────  VIN
GND            ──────  GND
GND            ──────  GAIN
D4  / GPIO25   ──────  DIN
D5  / GPIO26   ──────  BCLK
D6  / GPIO27   ──────  LRC
               SPK+ ─────────────────  +Loa
               SPK- ─────────────────  −Loa
```

Bộ **GPIO25/26/27** là chuẩn phổ biến nhất và an toàn nhất cho MAX98357A trên ESP32.

---

## 5. ⚠️ Quan trọng: nhãn "D" trên board ≠ số GPIO

Đây là cái bẫy dễ gây nhầm nhất. Trên nhiều board ESP32 generic, nhãn `D4` chính là `GPIO4`. **Nhưng trên board này thì KHÔNG** — board remap nhãn:

| Nhãn trên board | GPIO thật |
|-----------------|-----------|
| **D4** | **GPIO25** |
| **D5** | **GPIO26** |
| **D6** | **GPIO27** |

Vì vậy, khi nối dây **D4 → DIN**, thực chất là **GPIO25 → DIN**. Trong code ta dùng số GPIO thật (`25/26/27`).

> **Kinh nghiệm:** Khi ai đó nói "dùng chân D<n>", đừng tự coi đó là GPIO<n>. Phải đối chiếu sơ đồ của đúng board đó. Ở đây nếu tưởng D6 = GPIO6 thì sẽ tưởng đó là chân flash (cấm) — trong khi D6 thực ra là GPIO27, an toàn.

---

## 6. Bảng chân ESP32 — an toàn và cấm

| GPIO | Tình trạng | Dùng được? |
|------|-----------|------------|
| 0, 2, 5, 12, 15 | Strapping (ảnh hưởng lúc boot) | Hạn chế — tránh hoặc cẩn thận |
| 1, 3 | UART0 (dùng để nạp code) | ❌ Tránh |
| **6, 7, 8, 9, 10, 11** | **Nối với flash memory** | ❌❌ **Cấm tuyệt đối** |
| 34, 35, 36, 39 | Chỉ nhập (input only) | ❌ Không xuất được |
| **4, 13, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33** | Sạch, dùng thoải mái | ✅ |

Bộ `25/26/27` nằm trong nhóm an toàn.

---

## 7. Cách hoạt động

### 7.1. Giao tiếp I2S (3 dây)
I2S (Inter-IC Sound) truyền âm thanh số bằng 3 dây:

- **BCLK** (Bit Clock): xung clock cho từng bit dữ liệu.
- **LRC / WS** (Word Select): báo biết bit hiện tại thuộc kênh trái hay phải.
- **DIN** (Data In): bản thân dữ liệu âm thanh.

ESP32 làm **Master** (phát xung clock), MAX98357A làm **Slave** (nhận). MAX98357A có bộ DAC + khuếch đại lớp-D bên trong, tự giải mã I2S thành âm thanh analog đẩy ra loa.

### 7.2. Phát tone (sin 1kHz)
Code tạo ra một dãy mẫu (sample) sóng sin tần số 1 kHz:
- Tần số lấy mẫu: 16000 Hz.
- Độ phân giải: 16 bit.
- Biên độ: 3000 (trong khoảng 0–32767) — âm lượng vừa.

Mỗi vòng `loop()`, ESP32 tính 256 mẫu sin rồi đẩy ra I2S bằng `i2s_write()`. MAX98357A nhận liên tục → loa kêu tone 1 kHz đều.

---

## 8. Giải thích mã nguồn (`src/main.cpp`)

Khởi tạo I2S với cấu hình:
- `I2S_MODE_MASTER | I2S_MODE_TX` — ESP32 làm master, chỉ phát (TX).
- `I2S_CHANNEL_FMT_ONLY_LEFT` — dùng một kênh (MAX98357A ghép cả hai kênh về một loa).
- `dma_buf_count = 8`, `dma_buf_len = 64` — bộ đệm DMA để âm thanh liên tục không bị đứt.

Gán 3 chân I2S:
```cpp
.bck_io_num   = MAX98357_BCLK,   // BCLK  = GPIO26
.ws_io_num    = MAX98357_LRC,    // LRC   = GPIO27
.data_out_num = MAX98357_DIN,    // DIN   = GPIO25
.data_in_num  = I2S_PIN_NO_CHANGE
```

Trong `loop()`, tính sóng sin rồi `i2s_write()` liên tục.

---

## 9. Cấu hình PlatformIO (`platformio.ini`)

```ini
[env:esp32dev]
platform = espressif32@6.9.0   ; ghim bản 6.x
board = esp32dev
framework = arduino
monitor_speed = 115200
upload_speed = 115200
```

> **Vì sao ghim `espressif32@6.9.0`?**
> Code dùng driver I2S cũ (`driver/i2s.h`, gọi là *legacy API*). API này chỉ còn ở **Arduino-ESP32 core 2.x** (tương ứng platform `espressif32` 5.x–6.x). Nếu để `platform = espressif32` (mặc định mới nhất, 8.x → core 3.x), `driver/i2s.h` cũ bị bỏ → lỗi build. Nên ghim 6.9.0 cho chắc.

---

## 10. Cách Build & Upload

1. Cắm ESP32 vào máy qua USB-C.
2. Mở VS Code → mở folder `test MAX98357A loa`.
3. Ở thanh trạng thái dưới cùng:
   - **Build (✓)**: biên dịch kiểm tra.
   - **Upload (➔)**: nạp vào ESP32.
   - **Serial Monitor (🔌)**: mở cửa sổ xem log (115200 baud).

Hoặc bằng terminal:
```bash
pio run                # build
pio run -t upload      # nạp
pio device monitor     # xem serial
```

---

## 11. Kết quả mong đợi

Sau khi nạp xong:
- **Serial Monitor** in:
  ```
  ====================================
    Test MAX98357A - Tone 1kHz
    DIN=25  BCLK=26  LRC=27  (D4/D5/D6)
  ====================================
  I2S khoi tao xong -> bat dau phat am thanh.
  ```
- **Loa kêu "beeeep" liên tục** tần số ~1 kHz.

→ Nếu nghe được tiếng này = cả chuỗi phần cứng OK.

---

## 12. Khắc phục sự cố (Troubleshooting)

### A. Build lỗi `expected unqualified-id before numeric constant` ở dòng `TWO_PI`
`TWO_PI` là **macro có sẵn** trong `Arduino.h`. Không được khai báo biến/hằng trùng tên.
**Fix:** đổi tên biến (code này đã dùng `TWO_PI_VAL`).

### B. Build lỗi `Arduino.h not found` / các kiểu `i2s_config_t` không nhận
Đây là **lỗi ảo của IntelliSense** (clang) trước lần build đầu tiên, do VS Code chưa biết include path của PlatformIO. **Không phải lỗi code.** Sau lần build thành công đầu, PlatformIO tự sinh `.vscode/c_cpp_properties.json` → lỗi đỏ hết.

### C. Build lỗi nhiều vì I2S / không nhận `driver/i2s.h`
Platform đang ở bản 8.x (Arduino core 3.x). **Fix:** ghim `platform = espressif32@6.9.0` trong `platformio.ini`.

### D. Loa không kêu
Kiểm tra theo thứ tự:
1. **GND chung** giữa ESP32 và MAX98357A chưa?
2. **Nguồn VIN**: đã nối 3V3/5V? Module có đèn nguồn không?
3. **3 dây I2S đúng chưa**: DIN/BCLK/LRC không đảo, không khớp nhầm sang chân khác.
4. **Loa 4Ω/8Ω**, nối đúng SPK+/SPK−.
5. **GAIN**: nếu nối GAIN lên nguồn thì gain bị tắt → nối GAIN xuống GND.

### E. ESP32 không nạp được (Failed to connect)
Có thể do đang cắm dây vào **GPIO0/2/3** (strapping/UART nạp). Với bộ 25/26/27 thì không gặp vấn đề này. Nếu vẫn lỗi: giữ nút **BOOT** → bấm **RESET** → thả RESET → thả BOOT khi IDE báo "Connecting".

---

## 13. Mở rộng: phát file MP3 từ thẻ SD

Sau khi test tone OK, bước tiếp theo thường là phát MP3. Dùng thư viện **ESP32-audioI2S** (`schreibfaul1/ESP32-audioI2S`), code đơn giản hơn nhiều so với driver I2S tay:

```cpp
#include "Audio.h"
#include "SD.h"
#include "SPI.h"

#define I2S_BCLK 26
#define I2S_LRC  27
#define I2S_DOUT 25

// chân SPI cho module thẻ SD
#define SD_CS    5
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK  18

Audio audio;

void setup() {
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  Serial.begin(115200);

  if (!SD.begin(SD_CS)) {
    Serial.println("Loi the SD!");
    while (true);
  }

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(5);                      // 0..21, bắt đầu nhỏ
  audio.connecttoFS(SD, "/MYMUSIC.mp3");
}

void loop() {
  audio.loop();
}
```

> Lưu ý: thư viện này cũng cần platform 6.x (Arduino core 2.x) hoặc kiểm tra tương thích bản mới.

Tham khảo bài gốc: ESP32 MP3 Player (DroneBot Workshop).
