#include <Arduino.h>
#include <driver/i2s.h>
#include <math.h>

/*
 * Test MAX98357A (I2S Amplifier) - phat tone 1kHz lien tuc.
 *
 * Nối dây (ESP32 -> MAX98357A):
 *   3V3  -> VIN
 *   GND  -> GND
 *   GND  -> GAIN
 *   D4 / GPIO25 -> DIN
 *   D5 / GPIO26 -> BCLK
 *   D6 / GPIO27 -> LRC
 *   Loa   -> SPK+ / SPK-
 *
 * (Tren board cua ban: D4=GPIO25, D5=GPIO26, D6=GPIO27.)
 */

// Ten chan theo dung module MAX98357A
#define MAX98357_DIN   25   // ESP32 xuat du lieu -> noi vao DIN cua module
#define MAX98357_BCLK  26
#define MAX98357_LRC   27

#define I2S_PORT      I2S_NUM_0
#define SAMPLE_RATE   16000
#define TONE_FREQ     1000.0   // tan so tone (Hz)
#define AMPLITUDE     3000     // bien do (0 - 32767); 3000 la am luong vua

const double TWO_PI_VAL = 2.0 * 3.14159265;   // dat ten khac de trung macro TWO_PI cua Arduino

const int BUF_SAMPLES = 256;
int16_t buf[BUF_SAMPLES];

void setupI2S() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num   = MAX98357_BCLK,   // BCLK
    .ws_io_num    = MAX98357_LRC,    // LRC
    .data_out_num = MAX98357_DIN,    // ESP32 data out -> DIN module
    .data_in_num  = I2S_PIN_NO_CHANGE
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config);
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println();
  Serial.println("====================================");
  Serial.println("  Test MAX98357A - Tone 1kHz");
  Serial.println("  DIN=25  BCLK=26  LRC=27  (D4/D5/D6)");
  Serial.println("====================================");

  setupI2S();
  Serial.println("I2S khoi tao xong -> bat dau phat am thanh.");
}

void loop() {
  static double t = 0;
  double step = (TWO_PI_VAL * TONE_FREQ) / SAMPLE_RATE;

  // Sin ba song 1kHz vao buffer
  for (int i = 0; i < BUF_SAMPLES; i++) {
    buf[i] = (int16_t)(sin(t) * AMPLITUDE);
    t += step;
  }

  // Xuat buffer ra I2S (chan DIN -> MAX98357A)
  size_t written = 0;
  i2s_write(I2S_PORT, buf, sizeof(buf), &written, portMAX_DELAY);
}
