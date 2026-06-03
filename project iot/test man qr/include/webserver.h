#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>

/**
 * Khởi tạo web server trên port 80
 */
void webserver_init();

/**
 * Cập nhật dữ liệu QR hiện tại
 * @param amount số tiền
 * @param weight_str trọng lượng
 */
void webserver_update_qr(unsigned long amount, const char* weight_str);

/**
 * Xử lý request (gọi trong loop)
 */
void webserver_handle();

/**
 * Dừng webserver
 */
void webserver_stop();

#endif // WEBSERVER_H
