#include <Arduino.h>
#include <U8x8lib.h>
#include <U8g2lib.h>

//#define DEBUG

U8X8_SH1106_128X64_NONAME_SW_I2C u8x8(PIN2, PIN0, U8X8_PIN_NONE);

const uint8_t led7seg2[332] U8X8_FONT_SECTION("led7seg2") = 
  "\60X\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\200\200\200\200\0\0\0\0\0\0\0\0\0\0"
  "\0\0\7\7\7\7\0\0\0\0\0\0\0\0\0\0\0\0\340\340\340\340\0\0\0\0\0\0\0\0\0\0"
  "\0\0\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\200\300\340\340\340\340\340\340\340\340\340\340\300\200"
  "\0\0\0\0\0\0\0\0\0\0\1\3\7\7\7\7\7\7\7\7\7\7\3\1\0\0\0\0\0\0\370\374"
  "\376\376\374\370\0\0\37\77\177\177\77\37\0\0\370\374\376\376\375\373\7\7\373\375\376\376\374\370\0\0\37\77"
  "\177\177\277\337\340\340\337\277\177\177\77\37\0\0\0\0\200\300\340\340\340\340\340\340\340\340\340\340\300\200\0\0"
  "\0\0\0\0\0\0\0\0\1\3\7\7\7\7\7\7\7\7\7\7\3\1\0\0\0\0\0\0\370\374\376\376"
  "\374\370\0\0\37\77\177\177\77\37\0\0\370\374\376\376\375\373\7\7\373\375\376\376\374\370\0\0\37\77\177\177"
  "\277\337\340\340\337\277\177\177\77\37\0";

const char PROGMEM map7seg_left[10][6][4] = {
  { /* 0 */ "ABC", "IEJ", "H0H", "G0G", "KBL", "DEF" },
  { /* 1 */ "000", "00G", "00H", "00G", "00H", "000" },
  { /* 2 */ "ABC", "DEJ", "ABL", "IEF", "KBC", "DEF" },
  { /* 3 */ "ABC", "DEJ", "ABL", "DEJ", "ABL", "DEF" },
  { /* 4 */ "000", "G0G", "KBL", "DEJ", "00H", "000" },
  { /* 5 */ "ABC", "IEF", "KBC", "DEJ", "ABL", "DEF" },
  { /* 6 */ "ABC", "IEF", "KBC", "IEJ", "KBL", "DEF" },
  { /* 7 */ "ABC", "DEJ", "00H", "00G", "00H", "000" },
  { /* 8 */ "ABC", "IEJ", "KBL", "IEJ", "KBL", "DEF" },
  { /* 9 */ "ABC", "IEJ", "KBL", "DEJ", "ABL", "DEF" },
};

#define DIGIT_LEFT 0
#define DIGIT_RIGHT 1

// void sprintf_uint8(char *buf, uint8_t number, bool zeroPad = false) {
//   char *ptr = buf;
//   uint8_t divisor = 100;

//   while (divisor > 0) {
//     uint8_t digit = number / divisor;
//     if (digit > 0 || zeroPad) {
//       *ptr++ = digit + 0x30;
//     }

//     number -= digit * divisor;
//     divisor /= 10;
//   }

//   *ptr = 0;
// }

void blink(unsigned long msec) {
#ifndef DEBUG
  digitalWrite(PIN1, HIGH);
  delay(msec);
  digitalWrite(PIN1, LOW);
  delay(msec);
#endif
}

void slowBlink(void) {
  blink(1000);
}

void fastBlink(void) {
  blink(100);
}

void drawDigit(uint8_t pos, uint8_t digit, uint8_t side) {
  if (digit < 0 || digit > 9) {
    return;
  }

  if (side != DIGIT_LEFT && side != DIGIT_RIGHT) {
    return;
  }

  uint8_t row = 2;
  uint8_t col = pos * 4 + (side == DIGIT_RIGHT ? 1 : 0);

  char val[4];
  for (uint8_t line = 0; line < 6; line++) {
    // copy digit from progmem
    for (uint8_t i = 0; i < sizeof(val) - 1; i++) {
      val[i] = pgm_read_byte_near(&map7seg_left[digit][line][i]);
    }
    val[sizeof(val) - 1] = 0;

    if (strlen(val) == 0) {
      continue;
    }

    if (side == DIGIT_RIGHT) {
      for (uint8_t i = 0; i < strlen(val); i++) {
        val[i] = val[i] + 12;
      }
    } 
      
#ifndef DEBUG
    u8x8.drawString(col, row + line, val);
#else
    Serial.print(col);
    Serial.print(row + line);
    Serial.print(val);
#endif
  }
}

void draw_dots(void) {
#ifndef DEBUG
  u8x8.drawString(7, 3, "12");
  u8x8.drawString(7, 4, "34");
  u8x8.drawString(7, 5, "56");
  u8x8.drawString(7, 6, "78");
#endif
}

void delete_dots(void) {
#ifndef DEBUG
  u8x8.drawString(7, 3, "00");
  u8x8.drawString(7, 4, "00");
  u8x8.drawString(7, 5, "00");
  u8x8.drawString(7, 6, "00");
#endif
}

//-----------------------------------------------------------------------------

uint8_t hour = 0;
uint8_t minute = 0;
uint8_t second = 0;

//-----------------------------------------------------------------------------

void setup(void) {
  pinMode(PIN1, OUTPUT);

#ifndef DEBUG
  u8x8.begin();

  u8x8.setFont(led7seg2);
  u8x8.setContrast(63);
#else
  Serial.begin(9600);
#endif

  char hhmmss[9] = __TIME__;
  hour = (hhmmss[0] - '0') * 10 + (hhmmss[1] - '0');
  minute = (hhmmss[3] - '0') * 10 + (hhmmss[4] - '0');
  second = (hhmmss[6] - '0') * 10 + (hhmmss[7] - '0');

  slowBlink();
}

//-----------------------------------------------------------------------------

bool refresh = true;

void loop(void) {
  if (refresh) {
    drawDigit(0, hour / 10, DIGIT_RIGHT);
    drawDigit(1, hour % 10, DIGIT_LEFT);
    drawDigit(2, minute / 10, DIGIT_RIGHT);
    drawDigit(3, minute % 10, DIGIT_LEFT);
  }

  if (second % 2) {
    draw_dots();
  } else {
    delete_dots();
  }

  refresh = false;
  if (second++ > 59) {
    second = 0;
    if (minute++ > 59) {
      minute = 0;
      if (hour++ > 23) {
        hour = 0;
      }
    }

    refresh = true;
  }

  delay(1000);
}
