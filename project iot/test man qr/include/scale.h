#ifndef SCALE_H
#define SCALE_H

#include <Arduino.h>

void scale_init();
float scale_read();
float scale_read_avg(uint8_t samples = 10);
void scale_tare();
void scale_calibrate(float known_weight);
long scale_get_offset();
void scale_set_offset(long offset);
float scale_get_factor();
void scale_set_factor(float factor);
bool scale_is_stable();
String scale_format_weight(float weight_gram);

#endif // SCALE_H
