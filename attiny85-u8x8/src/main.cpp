#include <Arduino.h>
#include <U8x8lib.h>

U8X8_SH1106_128X64_NONAME_SW_I2C u8x8(PIN2, PIN0, U8X8_PIN_NONE);
//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(PIN2, PIN0, U8X8_PIN_NONE);

void setup(void) {
  pinMode(PIN1, OUTPUT);

  u8x8.begin();

  u8x8.setFont(u8x8_font_chroma48medium8_r);
}

void loop(void) {
  u8x8.clear();
  for (int i = 0; i < 8; i++) {
    u8x8.drawString(0, i, "8888888888888888");
  }

  delay(5000);
  digitalWrite(1, HIGH);
  delay(1000);
  digitalWrite(1, LOW);
}
