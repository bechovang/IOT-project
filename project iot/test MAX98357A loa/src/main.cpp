#include <Arduino.h>
#include "Audio.h"
#include <LittleFS.h>

/*
 * Phat MP3 tu flash (LittleFS) qua MAX98357A - dung ESP32-audioI2S.
 *
 * Noi day (ESP32 -> MAX98357A):
 *   3V3  -> VIN
 *   GND  -> GND
 *   GND  -> GAIN
 *   GPIO25 (D4) -> DIN
 *   GPIO26 (D5) -> BCLK
 *   GPIO27 (D6) -> LRC
 *   Loa  -> SPK+ / SPK-
 *
 * File am thanh: data/song.mp3
 *   -> phai upload vao flash truoc: PlatformIO -> "Upload Filesystem Image"
 *      (hoac lenh: pio run -t uploadfs)
 */

#define I2S_BCLK  26
#define I2S_LRC   27
#define I2S_DOUT  25
#define VOLUME    8      // 0..21; bat dau nho cho on tai voi loa nho
#define SONG_FILE "/song.mp3"

Audio audio;

// Co bao het bai -> phat lai (loop). Dung flag thay vi goi trong callback.
volatile bool replayFlag = false;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println();
  Serial.println("====================================");
  Serial.println("  MAX98357A - MP3 Player (LittleFS)");
  Serial.println("  DIN=25  BCLK=26  LRC=27  (D4/D5/D6)");
  Serial.println("====================================");

  // 1. Mount LittleFS (chua /song.mp3)
  if (!LittleFS.begin(true)) {
    Serial.println("LOI: khong mount duoc LittleFS!");
    Serial.println("-> Ban da chay 'Upload Filesystem Image' chua?");
    while (true) { delay(1000); }
  }
  Serial.println("LittleFS mount OK.");

  // 2. Cau hinh chan I2S va am luong
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(VOLUME);

  // 3. Mo file va bat dau phat
  if (audio.connecttoFS(LittleFS, SONG_FILE)) {
    Serial.printf("Dang phat: %s\n", SONG_FILE);
  } else {
    Serial.println("LOI: khong mo duoc file. Kiem tra /song.mp3 co trong LittleFS khong.");
  }
}

void loop() {
  audio.loop();

  // Het bai -> phat lai tu dau
  if (replayFlag) {
    replayFlag = false;
    Serial.println("Loop: phat lai tu dau.");
    audio.connecttoFS(LittleFS, SONG_FILE);
  }
}

// ================= Callbacks cua thu vien Audio =================
// In thong tin (sample rate, kenh...) ra Serial de debug.
void audio_info(const char *info) {
  Serial.print("info        : "); Serial.println(info);
}

// In tag MP3 (ten bai, tac gia...).
void audio_id3data(const char *info) {
  Serial.print("id3data     : "); Serial.println(info);
}

// Het file MP3 -> danh dau phat lai.
void audio_eof_mp3(const char *info) {
  Serial.print("eof_mp3     : "); Serial.println(info);
  replayFlag = true;
}
