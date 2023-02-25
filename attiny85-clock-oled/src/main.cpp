#include <Arduino.h>
#include <U8x8lib.h>
#include <U8g2lib.h>

//#define DEBUG

U8X8_SH1106_128X64_NONAME_SW_I2C u8x8(PIN2, PIN0, U8X8_PIN_NONE);

const uint8_t led7seg2[348] U8X8_FONT_SECTION("led7seg2") = 
  "\60Z\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\200\200\200\200\0\0\0\0\0\0\0\0\0\0"
  "\0\0\7\7\7\7\0\0\0\0\0\0\0\0\0\0\0\0\340\340\340\340\0\0\0\0\0\0\0\0\0\0"
  "\0\0\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
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

void blink(int msec) {
  digitalWrite(PIN1, HIGH);
  delay(msec);
  digitalWrite(PIN1, LOW);
  delay(msec);
}

void slowBlink() {
  blink(1000);
}

void fastBlink() {
  blink(100);
}

void setup(void) {
  pinMode(PIN1, OUTPUT);

#ifndef DEBUG
  u8x8.begin();

  u8x8.setFont(led7seg2);
  u8x8.setContrast(127);
#endif

  slowBlink();
}

const char map7seg_left[10][6][4] = {
  { /* 0 */ "ABC", "IEJ", "H0H", "G0G", "KBL", "DEF" },
  { /* 1 */ "000", "00G", "00H", "00G", "00H", "000" },
  { /* 2 */ "ABC", "DEJ", "ABL", "IEF", "KB0", "DEF" },
  { /* 3 */ "ABC", "DEJ", "ABL", "DEL", "ABJ", "DEF" },
  { /* 4 */ "000", "G0G", "KBK", "DEJ", "00H", "000" },
  { /* 5 */ "ABC", "IEF", "KBC", "DEJ", "ABL", "DEF" },
  { /* 6 */ "ABC", "IEF", "KBC", "IEJ", "KBL", "DEF" },
  { /* 7 */ "ABC", "DEJ", "00H", "00G", "00H", "000" },
  { /* 8 */ "ABC", "IEJ", "KBL", "IEJ", "KBL", "DEF" },
  { /* 9 */ "ABC", "IEJ", "KBL", "DEL", "ABL", "DEF" },
};

#define DIGIT_LEFT 0
#define DIGIT_RIGHT 1

void drawDigit(unsigned int pos, unsigned int digit, unsigned int side) {
  if (digit < 0 || digit > 9) {
    return;
  }

  if (side != DIGIT_LEFT && side != DIGIT_RIGHT) {
    return;
  }

  int row = 2;
  int col = pos * 4 + (side == DIGIT_RIGHT ? 1 : 0);

  for (unsigned int line = 0; line < 6; line++) {
    char val[4] = "";

    strcpy(val, map7seg_left[digit][line]);

    if (side == DIGIT_RIGHT) {
      for (unsigned int i = 0; i < strlen(val); i++) {
        val[i] = val[i] + 24;
      }
    } 
  
    if (strlen(val) == 0) {
      continue;
    }
    
#ifndef DEBUG
    u8x8.drawString(col, row + line, val);
#endif
  }
}

void draw_dots(void) {
  u8x8.drawString(7, 3, "12");
  u8x8.drawString(7, 4, "34");
  u8x8.drawString(7, 5, "56");
  u8x8.drawString(7, 6, "78");
}

unsigned int digit = 0;

void loop(void) {
  fastBlink();

#ifndef DEBUG
  u8x8.clear();
#endif

  drawDigit(0, digit, DIGIT_RIGHT);
  drawDigit(1, digit, DIGIT_LEFT);
  if (digit % 2) {
    draw_dots();
  }
  drawDigit(2, digit, DIGIT_RIGHT);
  drawDigit(3, digit, DIGIT_LEFT);

  if (++digit > 9) {
    digit = 0;
  }

  delay(800);
}
