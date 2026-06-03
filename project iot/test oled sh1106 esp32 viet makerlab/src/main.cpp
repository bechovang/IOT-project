#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

// Khởi tạo OLED SH1106 Hardware I2C (SDA=21, SCL=22)
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Hàm vẽ trái tim đặc tại (x, y) với kích thước S
void drawHeart(int x, int y, int S) {
  if (S <= 0) return;
  u8g2.drawDisc(x - S, y - S, S);
  u8g2.drawDisc(x + S, y - S, S);
  u8g2.drawTriangle(x - 2 * S, y - S, x + 2 * S, y - S, x, y + 2 * S);
}

// Cấu trúc cho hiệu ứng pháo hoa trái tim khi trúng mũi tên
struct Particle {
  float x;
  float y;
  float vx;
  float vy;
  bool active;
};

const int MAX_PARTICLES = 15;
Particle particles[MAX_PARTICLES];

// Trạng thái hoạt cảnh
enum State {
  ARROW_FLYING,   // Mũi tên đang bay tới
  HEART_HIT,      // Mũi tên bắn trúng trái tim -> Bùng nổ hạt
  MESSAGE_FADE    // Hiển thị lời tỏ tình ngọt ngào
};

State currentState = ARROW_FLYING;

// Tọa độ mũi tên tình yêu (Cupid's Arrow)
float arrowX = -30;
float arrowY = 45;
const float arrowTargetX = 64; // Tọa độ trái tim ở tâm
const float arrowTargetY = 30;
float arrowSpeedX = 2.5;
float arrowSpeedY = -0.4;

int heartSize = 0; // Trái tim xuất hiện từ nhỏ đến lớn
float heartBeatAngle = 0;
int explosionTimer = 0;
int textIndex = 0;
unsigned long lastTextTime = 0;

const char confessionText[] = "Em lam nguoi yeu anh nhe? <3";
char currentMessage[40] = "";

void triggerExplosion(int x, int y) {
  for (int i = 0; i < MAX_PARTICLES; i++) {
    particles[i].x = x;
    particles[i].y = y;
    // Bắn hạt ra các hướng ngẫu nhiên
    float angle = random(0, 360) * DEG_TO_RAD;
    float speed = random(10, 30) / 10.0;
    particles[i].vx = cos(angle) * speed;
    particles[i].vy = sin(angle) * speed;
    particles[i].active = true;
  }
}

void setup() {
  Wire.begin();
  u8g2.begin();
  randomSeed(analogRead(0));
}

void loop() {
  u8g2.clearBuffer();

  switch (currentState) {
    case ARROW_FLYING: {
      // 1. Trái tim hiện hình dần ở trung tâm
      if (heartSize < 7) {
        static int growthDelay = 0;
        if (++growthDelay % 5 == 0) heartSize++;
      }
      drawHeart(64, 30, heartSize);

      // 2. Vẽ mũi tên đang bay xiên từ góc trái dưới lên
      arrowX += arrowSpeedX;
      arrowY += arrowSpeedY;

      // Vẽ thân mũi tên (đoạn thẳng)
      u8g2.drawLine(arrowX, arrowY, arrowX - 15, arrowY + 2);
      // Vẽ đuôi mũi tên (lông vũ)
      u8g2.drawLine(arrowX - 15, arrowY + 2, arrowX - 18, arrowY + 5);
      u8g2.drawLine(arrowX - 15, arrowY + 2, arrowX - 18, arrowY - 1);
      // Vẽ đầu mũi tên
      u8g2.drawTriangle(arrowX, arrowY, arrowX - 4, arrowY - 2, arrowX - 4, arrowY + 2);

      // Khi đầu mũi tên chạm tới vùng của trái tim ở giữa màn hình
      if (arrowX >= arrowTargetX - 4) {
        currentState = HEART_HIT;
        triggerExplosion(arrowTargetX, arrowTargetY);
        explosionTimer = 0;
      }
      break;
    }

    case HEART_HIT: {
      // 1. Vẽ trái tim đang bị rung động mạnh
      int displaySize = 7 + (explosionTimer % 2 == 0 ? 1 : -1);
      drawHeart(64, 30, displaySize);

      // Vẽ mũi tên xuyên qua trái tim
      u8g2.drawLine(50, 32, 35, 34);
      u8g2.drawLine(78, 28, 88, 26);
      u8g2.drawTriangle(88, 26, 84, 24, 84, 28); // Đầu mũi tên ló ra bên phải

      // 2. Cập nhật và vẽ các hạt phát sáng bùng nổ
      bool anyActive = false;
      for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].active) {
          particles[i].x += particles[i].vx;
          particles[i].y += particles[i].vy;
          // Vẽ hạt
          u8g2.drawPixel(particles[i].x, particles[i].y);
          
          // Giảm độ hoạt động theo thời gian
          if (particles[i].x < 0 || particles[i].x > 128 || particles[i].y < 0 || particles[i].y > 64) {
            particles[i].active = false;
          } else {
            anyActive = true;
          }
        }
      }

      explosionTimer++;
      // Sau khoảng 45 khung hình, chuyển sang màn hình hiển thị tin nhắn
      if (explosionTimer > 45) {
        currentState = MESSAGE_FADE;
        textIndex = 0;
        currentMessage[0] = '\0';
        lastTextTime = millis();
      }
      break;
    }

    case MESSAGE_FADE: {
      // 1. Trái tim đập thong thả, đằm thắm ở phía trên
      int beatSize = 6 + sin(heartBeatAngle) * 1.5;
      heartBeatAngle += 0.1;
      drawHeart(64, 20, beatSize);

      // Vẽ mũi tên xuyên tim lãng mạn
      u8g2.drawLine(52, 22, 42, 23);
      u8g2.drawLine(76, 18, 86, 17);
      u8g2.drawTriangle(86, 17, 82, 15, 82, 19);

      // 2. Tạo hiệu ứng chữ gõ máy tính (Typewriter effect) cho lời tỏ tình
      if (textIndex < strlen(confessionText)) {
        if (millis() - lastTextTime > 150) { // Mỗi 150ms hiện thêm 1 chữ
          currentMessage[textIndex] = confessionText[textIndex];
          textIndex++;
          currentMessage[textIndex] = '\0';
          lastTextTime = millis();
        }
      }

      // Vẽ dòng chữ tỏ tình căn giữa bên dưới
      u8g2.setFont(u8g2_font_6x10_tf); // Font chữ rõ ràng, dễ đọc
      // Tính toán căn giữa màn hình cho tin nhắn
      int textWidth = u8g2.getStrWidth(currentMessage);
      u8g2.drawStr((128 - textWidth) / 2, 54, currentMessage);

      // Nhấp nháy biểu tượng nhỏ bên dưới sau khi gõ xong
      if (textIndex >= strlen(confessionText) && (millis() / 500) % 2 == 0) {
        u8g2.setFont(u8g2_font_unifont_t_vietnamese2);
        u8g2.drawStr(58, 64, "<3");
      }
      break;
    }
  }

  u8g2.sendBuffer();
  delay(20);
}
