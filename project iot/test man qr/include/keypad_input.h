#ifndef KEYPAD_INPUT_H
#define KEYPAD_INPUT_H

#include <Arduino.h>

/**
 * Khởi tạo keypad 4x4
 */
void keypad_init();

/**
 * Đọc phím đang nhấn (non-blocking)
 * @return ký tự phím, hoặc '\0' nếu không có
 */
char keypad_get_key();

/**
 * Nhập đơn giá từ keypad
 * @param buffer buffer chứa chuỗi số
 * @param max_len độ dài tối đa
 * @return true khi nhấn '#' (confirm), false nếu đang nhập
 */
bool keypad_input_price(char* buffer, uint8_t max_len);

/**
 * Xử lý phím đặc biệt
 * '*' = Xóa / Backspace
 * '#' = Confirm / Enter
 * 'A' = Tare cân
 * 'B' = Chuyển đổi đơn vị
 * 'C' = In lại QR
 * 'D' = Reset giao dịch
 */
void keypad_handle_special(char key);

/**
 * Lấy đơn giá hiện tại
 * @return đơn giá (VND/kg), 0 nếu chưa nhập
 */
unsigned long keypad_get_price();

/**
 * Reset buffer nhập giá
 */
void keypad_reset_input();

/**
 * Map phím keypad 4x4
 *  1 2 3 A
 *  4 5 6 B
 *  7 8 9 C
 *  * 0 # D
 */

#endif // KEYPAD_INPUT_H
