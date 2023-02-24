#include <Arduino.h>
#include <U8x8lib.h>
#include <U8g2lib.h>

U8X8_SH1106_128X64_NONAME_SW_I2C u8x8(PIN2, PIN0, U8X8_PIN_NONE);

const uint8_t led7seg2[348] U8X8_FONT_SECTION("led7seg2") = 
  "\60Z\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\340\340\340\340\340\340\0\0\0\0\0\0\0\0\0"
  "\0\7\7\7\7\7\7\0\0\0\0\0\0\0\0\0\0\0\300\300\300\300\0\0\0\0\0\0\0\0\0\0"
  "\0\0\3\3\3\3\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\200\300\340\340\340\340\340\340\340\340\340\340\300\200"
  "\0\0\0\0\0\0\0\0\0\0\1\3\7\7\7\7\7\7\7\7\7\7\3\1\0\0\0\0\0\0\370\374"
  "\376\376\374\370\0\0\37\77\177\177\77\37\0\0\370\374\376\376\375\373\7\7\373\375\376\376\374\370\0\0\37\77"
  "\177\177\277\337\340\340\337\277\177\177\77\37\0\0\0\0\200\300\340\340\340\340\340\340\340\340\340\340\300\200\0\0"
  "\0\0\0\0\0\0\0\0\1\3\7\7\7\7\7\7\7\7\7\7\3\1\0\0\0\0\0\0\370\374\376\376"
  "\374\370\0\0\37\77\177\177\77\37\0\0\370\374\376\376\375\373\7\7\373\375\376\376\374\370\0\0\37\77\177\177"
  "\277\337\340\340\337\277\177\177\77\37\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

void sprintf_uint8(char *buf, uint8_t number, bool zeroPad = false) {
  char *ptr = buf;
  uint8_t divisor = 100;

  while (divisor > 0) {
    uint8_t digit = number / divisor;
    if (digit > 0 || zeroPad) {
      *ptr++ = digit + 0x30;
    }

    number -= digit * divisor;
    divisor /= 10;
  }

  *ptr = 0;
}

void setup(void) {
  pinMode(PIN1, OUTPUT);

  u8x8.begin();

  u8x8.setFont(led7seg2);
  u8x8.setContrast(127);
}

unsigned char map7seg_left[10][6][5] = {
};

void draw8_left(int pos) {
  u8x8.drawString(pos * 4 + 0, 2, "ABC");
  u8x8.drawString(pos * 4 + 0, 3, "IEJ");
  u8x8.drawString(pos * 4 + 0, 4, "KBL");
  u8x8.drawString(pos * 4 + 0, 5, "IEJ");
  u8x8.drawString(pos * 4 + 0, 6, "KBL");
  u8x8.drawString(pos * 4 + 0, 7, "DEF");
}

void draw8_right(int pos) {
  u8x8.drawString(pos * 4 + 1, 2, "MNO");
  u8x8.drawString(pos * 4 + 1, 3, "UQV");
  u8x8.drawString(pos * 4 + 1, 4, "WNX");
  u8x8.drawString(pos * 4 + 1, 5, "UQV");
  u8x8.drawString(pos * 4 + 1, 6, "WNX");
  u8x8.drawString(pos * 4 + 1, 7, "PQR");
}

void draw_dots(void) {
  // u8x8.drawString(7, 3, "12");
  // u8x8.drawString(7, 4, "34");
  // u8x8.drawString(7, 5, "12");
  // u8x8.drawString(7, 6, "34");
  u8x8.drawString(7, 3, "56");
  u8x8.drawString(7, 4, "78");
  u8x8.drawString(7, 5, "56");
  u8x8.drawString(7, 6, "78");
}

void loop(void) {
  u8x8.clear();

  draw8_left(0);
  draw8_left(1);
  draw_dots();
  draw8_right(2);
  draw8_right(3);

  delay(5000);
  digitalWrite(1, HIGH);
  delay(1000);
  digitalWrite(1, LOW);
}
