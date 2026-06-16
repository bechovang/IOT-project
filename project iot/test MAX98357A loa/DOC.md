# Hướng dẫn kiểm tra loa MAX98357A với ESP32 (PlatformIO)

Tài liệu hướng dẫn kết nối, cấu hình và kiểm tra **mạch khuếch đại âm thanh I2S MAX98357A** với **ESP32** qua PlatformIO (VS Code). Code test phát một âm tone liên tục ~1 kHz để xác nhận toàn bộ chuỗi **ESP32 → I2S → MAX98357A → loa** hoạt động.

> 📌 **Cập nhật (chế độ hiện tại):** Project đang ở chế độ **phát MP3 từ flash LittleFS** (file `data/song.mp3`) — chi tiết ở [Mục 14](#14-che-do-phat-mp3-tu-flash-littlefs-hien-tai). Code test tone 1kHz cũ vẫn nằm trong git history.

> ✅ **Trạng thái (16/06):** Code + cấu hình đã **BUILD THÀNH CÔNG** (firmware 733KB, ảnh LittleFS OK). Chỉ còn việc **nạp vào board**.
>
> 🗓️ **Mai làm tiếp — đúng 3 bước:**
> 1. **Upload Filesystem Image** → đưa nhạc vào flash: `pio run -t uploadfs`
> 2. **Upload code** → `pio run -t upload`
> 3. **Reset** board → nghe nhạc (mở Serial Monitor 115200 xem log).
>
> ⚠️ Khi nạp: **tắt Serial Monitor** (đóng cổng COM) nếu đang mở, nếu không báo "port busy".

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

---

## 14. Chế độ phát MP3 từ flash LittleFS (hiện tại)

Project hiện chơi file MP3 (`data/song.mp3`) nạp trực tiếp vào flash ESP32, **không cần thẻ SD**. Dùng thư viện **ESP32-audioI2S** (`schreibfaul`) để giải mã MP3 + xuất I2S.

### 14.1. Vì sao phải nén MP3
ESP32-WROOM có **4MB flash**. File gốc `Spring-Flowers(chosic.com).mp3` là **3.3MB** (128kbps, stereo, 213s) — **không vừa**. Nén xuống **64kbps → ~1.7MB** để vừa vùng LittleFS (~2.19MB).

**Tạo lại `data/song.mp3`** (cần `ffmpeg`):
```bash
mkdir -p data
ffmpeg -y -i "sound/Spring-Flowers(chosic.com).mp3" -codec:a libmp3lame -b:a 64k -ac 2 "data/song.mp3"
```
> Nếu muốn nhạc khác: copy file vào `sound/`, chạy lệnh trên đổi `-i` cho đúng tên. Phải giữ tên ra là `data/song.mp3` (hoặc đổi `SONG_FILE` trong code).

### 14.2. Phân vùng flash (`partitions.csv`)
```
factory app   ~1.75MB   (firmware audio)
spiffs        ~2.19MB   (LittleFS, chứa /song.mp3)
```
Toàn bộ 4MB. `board_build.partitions = partitions.csv` + `board_build.filesystem = littlefs` trong `platformio.ini`.

### 14.3. Quy trình nạp (CÓ THỨ TỰ)
**Bắt buộc làm đủ 2 bước**, theo thứ tự:

1. **Upload filesystem** (đưa `data/song.mp3` vào flash):
   - VS Code: PlatformIO → **Upload Filesystem Image** (nút ở menu PlatformIO).
   - Hoặc terminal: `pio run -t uploadfs`
2. **Upload code** (firmware MP3 player):
   - Nút **Upload (➔)** hoặc `pio run -t upload`.

Sau đó **Reset** board → nghe nhạc. Bài tự phát lại (loop) khi hết.

> ⚠️ Nếu chỉ upload code mà quên uploadfs → Serial báo "không mở được /song.mp3".

### 14.4. Platform & thư viện (lưu ý quan trọng)
Vẫn dùng **`platform = espressif32@6.9.0`** (Arduino core 2.x, GCC 8.4) giống tone test. Phải pin thư viện về **bản 2.0.6**:
```
lib_deps = https://github.com/schreibfaul1/ESP32-audioI2S.git#2.0.6
```
> **Vì sao:** ESP32 toolchain GCC 8.4 **không có C++20** (thiếu header `<span>`). Bản ESP32-audioI2S mới (3.x) cần C++20 → build lỗi `fatal error: span: No such file or directory`. Bản 2.0.6 viết cho core 2.x nên build ngon. Đừng nâng lib lên 3.x trừ khi đổi sang platform có GCC ≥12.

### 14.5. Âm lượng
`#define VOLUME 8` (0..21). Bắt đầu nhỏ để ù tai với loa nhỏ; tăng dần nếu cần.

### 14.6. Khắc phục sự cố MP3
| Hiện tượng | Nguyên nhân / Fix |
|------------|-------------------|
| Serial: "không mở được /song.mp3" | Chưa chạy **Upload Filesystem Image**, hoặc sai tên file |
| Build lỗi `Audio.h` không tìm thấy | Chưa tải lib ESP32-audioI2S (PlatformIO tự tải khi build) |
| Build lỗi `fatal error: span: No such file or directory` | Đang dùng audio lib 3.x với GCC 8.4 → pin về `#2.0.6` (xem 14.4) |
| Build lỗi vùng app quá nhỏ | Firmware > 1.75MB → tăng phần `factory` trong `partitions.csv` (giảm `spiffs`) |
| File không vừa LittleFS | MP3 > 2.1MB → nén bitrate thấp hơn (VD `-b:a 48k`) |
| `uploadfs` báo lỗi littlefs tool | Xóa `.pio` rồi build lại (`pio run -t uploadfs`) |
| Không có tiếng | Như mục 12 (GND chung, nguồn VIN, GAIN, dây I2S đúng) |

---

## 15. Bộ nhớ ESP32 (Flash / SRAM / PSRAM)

ESP32 có **3 loại bộ nhớ**, rất hay nhầm lẫn:

| Loại | Dung lượng | Tính chất | Dùng để |
|------|-----------|-----------|---------|
| **Flash** (NAND) | **4MB** (ESP-WROOM-32E) | Bất biến — lưu khi tắt nguồn | Lưu **firmware + file data** (`song.mp3` nằm đây) |
| **SRAM** (RAM trong chip) | ~520KB (dùng được ~320KB) | Bay hơi — mất khi tắt | Biến, stack khi code chạy |
| **PSRAM** (RAM ngoài, *tùy board*) | 0 / 4MB / 8MB | Bay hơi | Bộ đệm lớn; **KHÔNG lưu được file** |

### 15.1. Vụ "nhớ là 50MB"
**50MB là nhớ nhầm.** ESP32-WROOM chuẩn chỉ có **4MB flash**. Khả năng bạn nhầm một trong các thứ:
- **MB (megabyte) vs Mb (megabit):** 4MB = 32Mb. Trên chip thường in `32Mbit` hoặc `4MByte`.
- Lẫn với file **`Anh Cu Di Di - Cakid.wav` (76MB)** trong thư mục `sound/`.
- Lẫn với **PSRAM** (đó là RAM, không phải chỗ cất nhạc).

### 15.2. Cách kiểm tra flash thật trên board
Cắm USB, **tắt hết Serial Monitor / app đang dùng cổng COM**, rồi chạy (Git Bash):

```bash
~/.platformio/penv/Scripts/python.exe \
  ~/.platformio/packages/tool-esptoolpy/esptool.py \
  --port COM3 --baud 115200 flash_id
```
(đổi `COM3` thành cổng ESP32 của bạn). Sẽ thấy dòng dạng:
```
Manufacturer: e0
Device: 4016
Detected flash size: 4MB
```
→ `Detected flash size` là dung lượng flash **thật** của chip.

### 15.3. Bố trí 4MB flash (`partitions.csv`)
```
bootloader + partition table   ~64KB
nvs + phy_init                 ~28KB
factory app                    ~1.75MB   ← firmware MP3 player (hiện 733KB, dư nhiều)
spiffs (LittleFS)              ~2.19MB   ← chứa /song.mp3 (1.7MB)
                               ─────────
                               = 4MB
```
Vì chỉ 4MB nên **phải nén MP3** (3.3MB → 1.7MB) mới vừa vùng LittleFS.

> 💡 **Nếu board thực ra có 8MB/16MB** (xác nhận bằng `flash_id` ở 15.2): sửa `partitions.csv` tăng vùng `spiffs` lên ~6–14MB → có thể dùng **MP3 gốc 3.3MB không cần nén**, hoặc chứa nhiều bài. Lúc đó nhớ đổi `board = esp32dev` thành board định nghĩa đúng flash (hoặc thêm `board_upload.flash_size`).

### 15.4. Theo dõi dung lượng khi build
Mỗi lần `pio run`, cuối output có 2 dòng:
```
RAM:   [=         ]  11.4% (used 37472 bytes from 327680 bytes)
Flash: [=         ]  xx.x%  (used ... bytes from ... bytes)
```
- **RAM** = SRAM (~320KB) dùng cho biến lúc chạy.
- **Flash** = phần app (firmware), so với vùng `factory` (~1.75MB), không phải toàn bộ chip.
- File nhạc nằm ở vùng `spiffs`, **không** nằm trong 2 con số này.


