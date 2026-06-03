# Hướng dẫn Kiểm tra ESP32 MakerLab với PlatformIO

Thư mục này chứa mã nguồn kiểm tra cơ bản dành cho bo mạch **ESP32** (tương thích các phiên bản của MakerLab).

---

## 1. Cấu trúc Dự án
* `platformio.ini`: File cấu hình môi trường cho dòng chip `esp32dev` với tốc độ Serial Monitor là `115200`.
* `src/main.cpp`: Mã nguồn kiểm tra kết hợp giữa **Nháy đèn LED (chân GPIO 2)** và **Quét tín hiệu WiFi (WiFi Scan)** để kiểm tra chip thu phát sóng RF.

---

## 2. Cách kiểm tra dự án

1. **Mở dự án trên VS Code**:
   * Mở VS Code -> Chọn biểu tượng **PlatformIO** -> Nhấp vào **Pick a Folder** (hoặc `File` -> `Open Folder...`).
   * Chọn đường dẫn: `C:\Users\Admin\Desktop\GIT CLONE\IOT-project\project iot\test esp32 viet makerlab`
2. **Cắm mạch ESP32** vào máy tính qua cáp USB.
3. **Biên dịch và nạp code (Upload)**:
   * Nhấn nút mũi tên sang phải **(→)** (Upload) ở dưới thanh trạng thái của VS Code để nạp code.
4. **Theo dõi Serial Monitor**:
   * Nhấn nút biểu tượng **Phích cắm/Kính viễn vọng** để mở Serial Monitor. 
   * Cấu hình baudrate trên Serial Monitor là **115200** để xem thông số kiểm tra LED và danh sách các mạng WiFi xung quanh quét được.
