#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <Arduino.h>

/**
 * Khởi tạo LCD1602 I2C
 */
void lcd_init();

/**
 * Hiển thị thành tiền cho khách hàng
 * @param weight_str chuỗi trọng lượng ("1.250 kg")
 * @param total thành tiền (VND)
 */
void lcd_show_total(const char* weight_str, unsigned long total);

/**
 * Hiển thị thông báo chờ
 */
void lcd_show_waiting();

/**
 * Hiển thị IP để quét QR
 * @param ip địa chỉ IP AP
 */
void lcd_show_ip(const char* ip);

/**
 * Hiển thị text tùy chọn
 * @param line1 dòng 1
 * @param line2 dòng 2
 */
void lcd_show_text(const char* line1, const char* line2);

#endif // LCD_DISPLAY_H
