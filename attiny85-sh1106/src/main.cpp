/*
  Uses code from:

  Tiny Graphics Library v3 - see http://www.technoblogy.com/show?23OS
  David Johnson-Davies - www.technoblogy.com - 23rd September 2018

  Adafruit TinyRTCLib
  https://github.com/adafruit/RTClib
*/

#include <Arduino.h>
#include <Wire.h>

//-----------------------------------------------------------------------------

void blink(unsigned long msec)
{
  digitalWrite(PIN1, HIGH);
  delay(msec);
  digitalWrite(PIN1, LOW);
  delay(msec);
}

void slowBlink(void)
{
  blink(1000);
}

void fastBlink(void)
{
  blink(100);
}

//-----------------------------------------------------------------------------

#define SH1106_ADDRESS 0x3c
#define SH1106_COMMAND 0x80
#define SH1106_DATA 0xC0
#define SH1106_COMMANDS 0x00
#define SH1106_DATAS 0x40

int SH1106_xpos;
int SH1106_ypos;

void Single(uint8_t data)
{
  Wire.write(SH1106_COMMAND);
  Wire.write(data);
}

void InitDisplay()
{
  Wire.beginTransmission(SH1106_ADDRESS);
  Wire.write(SH1106_COMMANDS);
  Wire.write(0xA1); // Flip horizontal
  Wire.write(0xAF); // Display on
  Wire.endTransmission();
}

void ClearDisplay()
{
  for (int p = 0; p < 8; p++)
  {
    Wire.beginTransmission(SH1106_ADDRESS);
    Single(0xB0 + p);
    Wire.endTransmission();

    for (int q = 0; q < 8; q++)
    {
      Wire.beginTransmission(SH1106_ADDRESS);
      Wire.write(SH1106_DATAS);

      for (int i = 0; i < 20; i++)
        Wire.write(0);

      Wire.endTransmission();
    }
  }
}

void PlotPoint(int x, int y, bool clear = false)
{
  Wire.beginTransmission(SH1106_ADDRESS);
  Single(0x00 + ((x + 2) & 0x0F));  // Column low nibble
  Single(0x10 + ((x + 2) >> 4));    // Column high nibble
  Single(0xB0 + (y >> 3));          // Page
  Single(0xE0);                     // Enter read modify write
  Wire.write(SH1106_DATA);
  Wire.endTransmission();

  Wire.requestFrom(SH1106_ADDRESS, 2);
  Wire.read();                      // Dummy read
  int j = Wire.read();

  Wire.beginTransmission(SH1106_ADDRESS);
  Wire.write(SH1106_DATA);
  uint8_t mask = 1 << (y & 0x07);
  if (clear)
    Wire.write(~mask & j);
  else
    Wire.write(mask | j);
  Single(0xEE);                     // Cancel read modify write
  Wire.endTransmission();
}

void MoveTo(int x, int y)
{
  SH1106_xpos = x;
  SH1106_ypos = y;
}

void DrawTo(int x, int y, bool clear = false)
{
  int sx, sy, e2, err;

  int dx = abs(x - SH1106_xpos);
  int dy = abs(y - SH1106_ypos);

  sx = (SH1106_xpos < x) ? 1 : -1;
  sy = (SH1106_ypos < y) ? 1 : -1;

  err = dx - dy;
  for (;;)
  {
    PlotPoint(SH1106_xpos, SH1106_ypos, clear);

    if (SH1106_xpos == x && SH1106_ypos == y)
      return;

    e2 = err << 1;

    if (e2 > -dy)
    {
      err = err - dy;
      SH1106_xpos = SH1106_xpos + sx;
    }
    else if (e2 < dx)
    {
      err = err + dx;
      SH1106_ypos = SH1106_ypos + sy;
    }
  }
}

void SetDisplayContrast(char contrast) {
  Single(0x81);
  Single(contrast);
}
//-----------------------------------------------------------------------------

void DrawVerticalSegment(uint8_t x, uint8_t y, bool clear = false)
{
  MoveTo(x + 0, y + 2); DrawTo(x + 0, y + 11, clear);
  MoveTo(x + 1, y + 1); DrawTo(x + 1, y + 12, clear);
  MoveTo(x + 2, y + 0); DrawTo(x + 2, y + 13, clear);
  MoveTo(x + 3, y + 0); DrawTo(x + 3, y + 13, clear);
  MoveTo(x + 4, y + 1); DrawTo(x + 4, y + 12, clear);
  MoveTo(x + 5, y + 2); DrawTo(x + 5, y + 11, clear);
}

void DrawHorizontalSegment(uint8_t x, uint8_t y, bool clear = false)
{
  MoveTo(x + 2, y + 0); DrawTo(x + 11, y + 0, clear);
  MoveTo(x + 1, y + 1); DrawTo(x + 12, y + 1, clear);
  MoveTo(x + 0, y + 2); DrawTo(x + 13, y + 2, clear);
  MoveTo(x + 0, y + 3); DrawTo(x + 13, y + 3, clear);
  MoveTo(x + 1, y + 4); DrawTo(x + 12, y + 4, clear);
  MoveTo(x + 2, y + 5); DrawTo(x + 11, y + 5, clear);
}

// orientation, x, y
const uint8_t PROGMEM segment_offsets[7][3] = {
  { 0, 4,  32 },
  { 1, 16, 20 },
  { 1, 16, 4  },
  { 0, 4,  0  },
  { 1, 0,  4  },
  { 1, 0,  20 },
  { 0, 4,  16 }
};

void DrawSegment(uint8_t x, uint8_t y, uint8_t segment, bool clear= false)
{
  if (segment >= 'a' && segment <= 'g')
   segment -= 'a';

  if (segment > 7)
   return;

  uint8_t o = pgm_read_byte(&segment_offsets[segment][0]);
  uint8_t ox = pgm_read_byte(&segment_offsets[segment][1]);
  uint8_t oy = pgm_read_byte(&segment_offsets[segment][2]);

  if (o == 0)
    DrawHorizontalSegment(x + ox, y + oy, clear);
  else
    DrawVerticalSegment(x + ox, y + oy, clear);
}

const uint8_t PROGMEM digit_segments[10][8] = {
  "abcdef",
  "bc",
  "abged",
  "abcdg",
  "bcfg",
  "acdfg",
  "acdefg",
  "abc",
  "abcdefg",
  "abcdfg"
};

void DrawDigit(uint8_t x, uint8_t y, uint8_t digit, bool clear = false)
{
  if (digit >= '0' && digit <= '9')
    digit -= '0';

  if (digit > 9)
    return;

  for (uint8_t i = 0; i < 8; i++)
  {
    uint8_t c = pgm_read_byte(&digit_segments[digit][i]);
    if (c == 0)
      break;

    DrawSegment(x, y, c, clear);
  }
}

void DrawDot(uint8_t x, uint8_t y, uint8_t size, bool clear = false)
{
  for (uint8_t i = 0; i < size; i++)
  {
    MoveTo(x, y + i);
    DrawTo(x + size - 1, y + i, clear);
  }
}

void DrawDots(bool clear = false)
{
  // DrawDot(61, 13, 6, clear);
  // DrawDot(61, 24, 6, clear);
  DrawDot(61, 5 + 14, 4, clear);
  DrawDot(61, 5 + 25, 4, clear);
}

void DrawDigitPos(uint8_t pos, uint8_t digit, bool clear = false)
{
  switch (pos)
  {
    case 0:
      DrawDigit(8, 5, digit, clear);
      break;
    case 1:
      DrawDigit(34, 5, digit, clear);
      break;
    case 2:
      DrawDigit(64 + 8, 5, digit, clear);
      break;
    case 3:
      DrawDigit(64 + 34, 5, digit, clear);
      break;
  }
}

// void DrawDigits(uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4)
// {
//   DrawDigitPos(0, digit1);
//   DrawDigitPos(1, digit2);
//   DrawDigitPos(2, digit3);
//   DrawDigitPos(3, digit4);
// }

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
  switch (date[0])
  {
  case 'J':
    if (date[1] == 'a')
      m = 1;
    else 
      m = date[2] == 'n' ? 6 : 7;
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
  default:
    // oh no
    m = 1;
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

#define BH1759_ADDRESS 0x23

void LightInit()
{
  Wire.beginTransmission(BH1759_ADDRESS);
  Wire.write(0x10); // Set mode
  Wire.endTransmission();
}

uint16_t LightGetIntensity(void)
{
  uint16_t Value = 0;

  Wire.requestFrom(BH1759_ADDRESS, 2);
  Value = Wire.read();
  Value <<= 8;
  Value |= Wire.read();

  return Value / 1.2;
}

//-----------------------------------------------------------------------------

void setup()
{
  slowBlink();

  Wire.begin();
  fastBlink();

  if (!RtcIsRunning()) {
    slowBlink();
    RtcAdjust(DateTime(__DATE__, __TIME__));
  }

  ClearDisplay();
  InitDisplay();
  SetDisplayContrast(127);
  fastBlink();

  LightInit();
  fastBlink();

  slowBlink();
}

//-----------------------------------------------------------------------------

uint8_t oldDigits[4] = {0xff, 0xff, 0xff, 0xff};
uint8_t digits[4];
bool dots = true;
uint8_t desiredContrast = 127;
uint8_t contrast = 127;

void loop()
{
  DateTime dt = RtcNow();
  digits[0] = dt.hour() / 10;
  digits[1] = dt.hour() % 10;
  digits[2] = dt.minute() / 10;
  digits[3] = dt.minute() % 10;

  for (uint8_t i = 0; i < 4; i++)
  {
    if (digits[i] != oldDigits[i])
    {
      if (oldDigits[i] != 0xff)
        DrawDigitPos(i, oldDigits[i], true);

      DrawDigitPos(i, digits[i], false);
      oldDigits[i] = digits[i];
    }
  }

  DrawDots(dots);
  dots = !dots;

  uint16_t light = LightGetIntensity();
  if (light < 100)
    desiredContrast = 15;
  else
    if (light > 10000)
      desiredContrast = 127;
    else
      desiredContrast = 15 + (light - 100) / ((10000 - 100) / (127 - 15));

  if (abs(contrast - desiredContrast) > 8) {
    contrast += (contrast < desiredContrast) ? 8 : -8;
    SetDisplayContrast(contrast);
  }

  delay(1000);
}
