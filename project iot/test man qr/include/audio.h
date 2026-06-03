#ifndef AUDIO_H
#define AUDIO_H

#include <Arduino.h>

/**
 * Khởi tạo I2S cho MAX98357A
 */
void audio_init();

/**
 * Phát beep ngắn
 * @param duration_ms thời gian (ms)
 * @param freq tần số (Hz), mặc định 1000
 */
void audio_beep(uint16_t duration_ms = BEEP_DURATION_MS, uint16_t freq = 1000);

/**
 * Phát beep báo giá (2 tiếng bíp)
 */
void audio_beep_price();

/**
 * Phát beep xác nhận (1 tiếng bíp dài)
 */
void audio_beep_confirm();

/**
 * Phát beep lỗi (3 tiếng bíp ngắn)
 */
void audio_beep_error();

/**
 * Phát tone tùy chỉnh
 * @param freq tần số Hz
 * @param duration_ms thời gian ms
 */
void audio_play_tone(uint16_t freq, uint16_t duration_ms);

#endif // AUDIO_H
