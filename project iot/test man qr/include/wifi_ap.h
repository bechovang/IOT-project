#ifndef WIFI_AP_H
#define WIFI_AP_H

#include <Arduino.h>

/**
 * Khởi tạo WiFi Access Point
 * @return IP address của AP
 */
String wifi_ap_init();

/**
 * Lấy IP hiện tại
 * @return IP string
 */
String wifi_ap_get_ip();

/**
 * Kiểm tra có client nào đang kết nối không
 * @return số lượng station đang kết nối
 */
int wifi_ap_get_clients();

/**
 * Kiểm tra station mới kết nối (mỗi lần gọi chỉ return true 1 lần)
 * @return true nếu có station mới
 */
bool wifi_ap_has_new_client();

#endif // WIFI_AP_H
