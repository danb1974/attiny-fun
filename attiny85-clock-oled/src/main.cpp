#include <Arduino.h>
#include <U8x8lib.h>
#include <U8g2lib.h>

U8X8_SH1106_128X64_NONAME_SW_I2C u8x8(PIN2, PIN0, U8X8_PIN_NONE);

const uint8_t led7seg[101] U8X8_FONT_SECTION("led7seg") = 
  "\60;\1\1\0\200\300\340\340\340\340\340\340\340\340\340\340\340\300\200\0\1\3\7\7\7\7\7\7\7\7\7"
  "\7\7\3\1\0\0\0\0\0\0\370\374\0\0\0\0\0\0\37\77\376\374\370\0\0\0\0\0\177\77\37\0"
  "\0\0\0\0\177\277\337\340\340\340\340\340\340\340\340\340\340\340\337\277\376\375\373\7\7\7\7\7\7\7\7\7"
  "\7\7\373\375";

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

  u8x8.setFont(led7seg);
  u8x8.setContrast(127);
}

unsigned char map7seg[10][6][5] = {
  {
    {"    "},
    {"  46"},
    {"  57"},
    {"  46"},
    {"  57"},
    {"    "},
  },
  {

  },
  {

  },
  {

  },
  {

  },
  {

  },
  {

  },
  {

  },
  {
    {" 01 "},
    {"4:;6"},
    {"5897"},
    {"4:;6"},
    {"5897"},
    {" 23 "},
  },
  {

  },
};

void draw1(int pos) {
  u8x8.drawString(pos * 4 + 2, 3, "46");
  u8x8.drawString(pos * 4 + 2, 4, "57");
  u8x8.drawString(pos * 4 + 2, 5, "46");
  u8x8.drawString(pos * 4 + 2, 6, "57");
}

void draw8(int pos) {
  u8x8.drawString(pos * 4 + 1, 2, "01");
  u8x8.drawString(pos * 4 + 0, 3, "4:;6");
  u8x8.drawString(pos * 4 + 0, 4, "5897");
  u8x8.drawString(pos * 4 + 0, 5, "4:;6");
  u8x8.drawString(pos * 4 + 0, 6, "5897");
  u8x8.drawString(pos * 4 + 1, 7, "23");
}

void loop(void) {
  u8x8.clear();

  draw1(0);
  draw8(1);
  draw8(2);
  draw8(3);

  delay(5000);
  digitalWrite(1, HIGH);
  delay(1000);
  digitalWrite(1, LOW);
}
