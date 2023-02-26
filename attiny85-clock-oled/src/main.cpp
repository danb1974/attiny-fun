#include <Arduino.h>
#include <U8x8lib.h>
#include <U8g2lib.h>
#include <Wire.h>

//#define DEBUG

class U8X8_SH1106_128X64_NONAME_SW_I2C_CUSTOM : public U8X8 {
  public: U8X8_SH1106_128X64_NONAME_SW_I2C_CUSTOM(uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8X8() {
    u8x8_Setup(getU8x8(), u8x8_d_sh1106_128x64_noname, u8x8_cad_ssd13xx_i2c, u8x8_byte_arduino_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};

//U8X8_SH1106_128X64_NONAME_SW_I2C u8x8(PIN2, PIN0, U8X8_PIN_NONE);
U8X8_SH1106_128X64_NONAME_SW_I2C_CUSTOM u8x8(PIN2, PIN0, U8X8_PIN_NONE);

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
//       *ptr++ = digit + '0';
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

class DateTime {
public:
  DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour = 0, uint8_t min = 0, uint8_t sec = 0);
  DateTime(const char *date, const char *time);
  uint16_t year() const { return y; }
  uint8_t month() const { return m; }
  uint8_t day() const { return d; }
  uint8_t hour() const { return hh; }
  uint8_t minute() const { return mm; }
  uint8_t second() const { return ss; }

protected:
  uint8_t y, m, d, hh, mm, ss;
};

static uint8_t conv2d(const char *p)
{
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

DateTime::DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
  y = year;
  m = month;
  d = day;
  hh = hour;
  mm = min;
  ss = sec;
}

DateTime::DateTime(const char *date, const char *time)
{
  // sample input: date = "Dec 26 2009", time = "12:34:56"
  y = conv2d(date + 7) * 100 + conv2d(date + 9);
  // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
  m = 1;
  switch (date[0])
  {
  case 'J':
    m = date[1] == 'a' ? 1 : (m = date[2] == 'n' ? 6 : 7);
    break;
  case 'F':
    m = 2;
    break;
  case 'A':
    m = date[2] == 'r' ? 4 : 8;
    break;
  case 'M':
    m = date[2] == 'r' ? 3 : 5;
    break;
  case 'S':
    m = 9;
    break;
  case 'O':
    m = 10;
    break;
  case 'N':
    m = 11;
    break;
  case 'D':
    m = 12;
    break;
  }
  d = conv2d(date + 4);

  hh = conv2d(time);
  mm = conv2d(time + 3);
  ss = conv2d(time + 6);
}

static uint8_t bcd2bin(uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd(uint8_t val) { return val + 6 * (val / 10); }

#define DS1307_ADDRESS 0x68

uint8_t RtcIsRunning()
{
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 1);
  uint8_t ss = Wire.read();
  return !(ss >> 7);
}

void RtcAdjust(const DateTime &dt)
{
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(0);
  Wire.write(bin2bcd(dt.second()));
  Wire.write(bin2bcd(dt.minute()));
  Wire.write(bin2bcd(dt.hour()));
  Wire.write(bin2bcd(0));
  Wire.write(bin2bcd(dt.day()));
  Wire.write(bin2bcd(dt.month()));
  Wire.write(bin2bcd(dt.year() - 2000));
  Wire.write(0);
  Wire.endTransmission();
}

DateTime RtcNow()
{
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 7);
  uint8_t ss = bcd2bin(Wire.read() & 0x7F);
  uint8_t mm = bcd2bin(Wire.read());
  uint8_t hh = bcd2bin(Wire.read());
  Wire.read();
  uint8_t d = bcd2bin(Wire.read());
  uint8_t m = bcd2bin(Wire.read());
  uint16_t y = bcd2bin(Wire.read()) + 2000;

  return DateTime(y, m, d, hh, mm, ss);
}

//-----------------------------------------------------------------------------

void setup(void) {
  pinMode(PIN1, OUTPUT);
 
#ifndef DEBUG
  // messes up i2c if after Wire.begin()
  u8x8.begin();
  u8x8.setFont(led7seg2);
  u8x8.setContrast(63);

  if (!RtcIsRunning()) {
    Wire.begin(); // because u8g2
    slowBlink();
    RtcAdjust(DateTime(__DATE__, __TIME__));
  }
#else
  Serial.begin(9600);
#endif

  slowBlink();
}

//-----------------------------------------------------------------------------

uint8_t oldDigits[4] = {0xff, 0xff, 0xff, 0xff};
uint8_t digits[4] = {0, 0, 0, 0};
bool dots = true;

void loop(void) {
#ifndef DEBUG
  Wire.begin(); // because u8g2
  DateTime dt = RtcNow();
  digits[0] = dt.hour() / 10;
  digits[1] = dt.hour() % 10;
  digits[2] = dt.minute() / 10;
  digits[3] = dt.minute() % 10;
#endif

  for (uint8_t i = 0; i < 4; i++)
  {
    if (digits[i] != oldDigits[i])
    {
      drawDigit(i, digits[i], i % 2 == 0 ? DIGIT_RIGHT : DIGIT_LEFT);
      oldDigits[i] = digits[i];
    }
  }

  if (dots) {
    draw_dots();
  } else {
    delete_dots();
  }
  dots = !dots;

  delay(1000);
}
