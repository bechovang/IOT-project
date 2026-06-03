#ifndef VIETQR_H
#define VIETQR_H

#include <Arduino.h>

/**
 * Sinh URL hình VietQR
 * @param amount số tiền (VND)
 * @param add_info mô tả thêm (VD: "CAN DIEN TU")
 * @return URL hình QR code
 */
String vietqr_gen_image_url(unsigned long amount, const char* add_info = "CAN DIEN TU");

/**
 * Sinh chuỗi QR content theo chuẩn VietQR (EMVCo)
 * Dùng khi muốn tự render QR trên web client
 * @param amount số tiền (VND)
 * @param add_info mô tả thêm
 * @return chuỗi QR payload
 */
String vietqr_gen_payload(unsigned long amount, const char* add_info = "CAN DIEN TU");

/**
 * Sinh trang HTML chứa QR code
 * @param amount số tiền
 * @param weight_str trọng lượng
 * @param ap_ip IP của AP
 * @return HTML string
 */
String vietqr_gen_html_page(unsigned long amount, const char* weight_str, const char* ap_ip);

/**
 * Format số tiền có dấu chấm phân cách
 * @param amount số tiền
 * @return string "15.000"
 */
String vietqr_format_money(unsigned long amount);

#endif // VIETQR_H
