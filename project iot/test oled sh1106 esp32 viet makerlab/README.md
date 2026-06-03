# Hướng dẫn kiểm tra OLED SH1106 với ESP32 MakerLab

Dự án PlatformIO này giúp kiểm tra màn hình OLED **SH1106 1.3 inch** sử dụng bo mạch **ESP32**.

---

## 1. Kết nối chân phần cứng (Sơ đồ mặc định)
* **VCC** -> 3.3V hoặc 5V trên ESP32.
* **GND** -> GND trên ESP32.
* **SCL** -> Chân **GPIO 22** trên ESP32.
* **SDA** -> Chân **GPIO 21** trên ESP32.

---

## 2. Điểm đặc trưng so với Arduino Uno
* **Bộ nhớ RAM lớn**: Vì ESP32 có lượng RAM rất lớn (520KB), chúng ta sử dụng chế độ nạp toàn khung hình (**Full Frame Buffer - `_F_`**) giúp hiển thị mượt mà hơn, vẽ đồ họa tốc độ cao mà không bị tràn bộ nhớ.
* **Quét I2C tự động**: Code có tích hợp chức năng tự quét địa chỉ I2C lúc khởi động để kiểm tra đường truyền phần cứng.

---

## 3. Cách mở và nạp code
1. Trong VS Code, chọn **File** -> **Open Folder...** -> Chọn thư mục `test oled sh1106 esp32 viet makerlab`.
2. Bấm nút nạp code **Upload (→)** ở thanh công cụ dưới để nạp chương trình vào ESP32.
3. Nếu ESP32 bị kẹt ở chế độ `DOWNLOAD_BOOT`, hãy nhấn nút **EN / RST** trên mạch một lần để khởi động lại mạch sau khi nạp.
