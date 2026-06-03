# Hướng dẫn Kiểm tra Arduino Uno với PlatformIO

Tài liệu này hướng dẫn cách cấu hình, nạp code và kiểm tra bo mạch **Arduino Uno** sử dụng **PlatformIO** (trên VS Code).

---

## 1. Cấu trúc Dự án
Dự án được cấu hình sẵn theo chuẩn PlatformIO:
* `platformio.ini`: File cấu hình môi trường, thông số board (Uno) và tốc độ Serial Monitor (`9600`).
* `src/main.cpp`: Mã nguồn C++ điều khiển nhấp nháy đèn LED tích hợp (chân 13) và gửi dữ liệu qua Serial.

---

## 2. Mã nguồn kiểm tra (`src/main.cpp`)
Trong môi trường PlatformIO C++, chúng ta bắt buộc phải khai báo thư viện `<Arduino.h>` ở đầu file:

```cpp
#include <Arduino.h>

void setup() {
  // Cấu hình LED chân 13 làm OUTPUT
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Khởi tạo Serial giao tiếp máy tính
  Serial.begin(9600);
  Serial.println("Arduino Uno (PlatformIO) khoi dong thanh cong!");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // Bật LED
  Serial.println("LED: BAT");
  delay(1000);                       // Chờ 1 giây
  
  digitalWrite(LED_BUILTIN, LOW);    // Tắt LED
  Serial.println("LED: TAT");
  delay(1000);                       // Chờ 1 giây
}
```

---

## 3. Cách sử dụng PlatformIO trong VS Code

### Bước 1: Chuẩn bị phần mềm
1. Tải và cài đặt [VS Code (Visual Studio Code)](https://code.visualstudio.com/).
2. Trong VS Code, mở phần **Extensions** (phím tắt `Ctrl+Shift+X`), tìm kiếm và cài đặt extension **PlatformIO IDE**.

### Bước 2: Mở dự án
1. Mở VS Code.
2. Chọn biểu tượng **PlatformIO** (hình đầu kiến/người ngoài hành tinh ở thanh menu bên trái).
3. Nhấp vào **Pick a Folder** (hoặc `File` -> `Open Folder...`) và mở thư mục:
   `C:\Users\Admin\Desktop\GIT CLONE\IOT-project\project iot\test arduino`

### Bước 3: Biên dịch và Nạp chương trình (Build & Upload)
Kết nối mạch Arduino Uno của bạn với máy tính qua cổng USB, sau đó sử dụng các công cụ ở góc dưới thanh trạng thái (Status Bar) của VS Code:

1. **Biên dịch (Build):**
   * Nhấn nút có biểu tượng **Dấu tích (✓)** để kiểm tra lỗi cú pháp và biên dịch code.
2. **Nạp chương trình (Upload):**
   * Nhấn nút có biểu tượng **Mũi tên sang phải (→)** để tải code vào chip Arduino Uno. PlatformIO sẽ tự động tìm kiếm cổng COM thích hợp.
3. **Theo dõi Serial (Serial Monitor):**
   * Nhấn nút có biểu tượng **Phích cắm** hoặc **Kính viễn vọng** để mở cửa sổ Serial Monitor. Bạn sẽ nhìn thấy các dòng thông báo `"LED: BAT"` và `"LED: TAT"` hiển thị liên tục theo trạng thái nhấp nháy của đèn trên mạch.
