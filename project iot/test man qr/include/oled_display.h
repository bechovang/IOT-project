#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Arduino.h>

/**
 * Khởi tạo OLED SH1106 1.3" qua I2C
 */
void oled_init();

/**
 * Hiển thị màn hình chính: trọng lượng + đơn giá + thành tiền
 * @param weight_gram trọng lượng (gram)
 * @param price_per_kg đơn giá (VND/kg)
 * @param total thành tiền (VND)
 */
void oled_show_main(float weight_gram, unsigned long price_per_kg, unsigned long total);

/**
 * Hiển thị màn hình nhập đơn giá
 * @param price_str chuỗi số đang nhập
 */
void oled_show_price_input(const char* price_str);

/**
 * Hiển thị QR code info
 * @param amount số tiền
 * @param ip địa chỉ IP để quét QR
 */
void oled_show_qr(unsigned long amount, const char* ip);

/**
 * Hiển thị thông báo
 * @param line1 dòng 1
 * @param line2 dòng 2 (có thể NULL)
 */
void oled_show_message(const char* line1, const char* line2 = NULL);

/**
 * Xóa màn hình
 */
void oled_clear();

#endif // OLED_DISPLAY_H
