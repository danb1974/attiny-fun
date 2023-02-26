/*
  Uses code from:

  Tiny Graphics Library v3 - see http://www.technoblogy.com/show?23OS
  David Johnson-Davies - www.technoblogy.com - 23rd September 2018
*/

#include <Arduino.h>
#include <Wire.h>

int const sda = PIN0;
int const scl = PIN2;

int const address = 0x3c;
int const commands = 0x00;
int const onecommand = 0x80;
int const data = 0x40;
int const onedata = 0xC0;

// OLED display **********************************************

int xpos;
int ypos;

void Single(uint8_t data)
{
  Wire.write(onecommand);
  Wire.write(data);
}

void InitDisplay()
{
  Wire.beginTransmission(address);
  Wire.write(commands);
  Wire.write(0xA1); // Flip horizontal
  Wire.write(0xAF); // Display on
  Wire.endTransmission();
}

void ClearDisplay()
{
  for (int p = 0; p < 8; p++)
  {
    Wire.beginTransmission(address);
    Single(0xB0 + p);
    Wire.endTransmission();

    for (int q = 0; q < 8; q++)
    {
      Wire.beginTransmission(address);
      Wire.write(data);
      for (int i = 0; i < 20; i++)
      {
        Wire.write(0);
      }
      Wire.endTransmission();
    }
  }
}

void PlotPoint(int x, int y)
{
  Wire.beginTransmission(address);
  Single(0x00 + ((x + 2) & 0x0F)); // Column low nibble
  Single(0x10 + ((x + 2) >> 4));   // Column high nibble
  Single(0xB0 + (y >> 3));         // Page
  Single(0xE0);                    // Read modify write
  Wire.write(onedata);
  Wire.endTransmission();
  Wire.requestFrom(address, 2);
  Wire.read(); // Dummy read
  int j = Wire.read();
  Wire.beginTransmission(address);
  Wire.write(onedata);
  Wire.write((1 << (y & 0x07)) | j);
  Single(0xEE); // Cancel read modify write
  Wire.endTransmission();
}

void MoveTo(int x, int y)
{
  xpos = x;
  ypos = y;
}

void DrawTo(int x, int y)
{
  int sx, sy, e2, err;

  int dx = abs(x - xpos);
  int dy = abs(y - ypos);

  sx = (xpos < x) ? 1 : -1;
  sy = (ypos < y) ? 1 : -1;

  err = dx - dy;
  for (;;)
  {
    PlotPoint(xpos, ypos);

    if (xpos == x && ypos == y)
      return;

    e2 = err << 1;

    if (e2 > -dy)
    {
      err = err - dy;
      xpos = xpos + sx;
    }

    if (e2 < dx)
    {
      err = err + dx;
      ypos = ypos + sy;
    }
  }
}

//-----------------------------------------------------------------------------

void DrawVerticalSegment(uint8_t x, uint8_t y)
{
  MoveTo(x + 0, y + 2); DrawTo(x + 0, y + 11);
  MoveTo(x + 1, y + 1); DrawTo(x + 1, y + 12);
  MoveTo(x + 2, y + 0); DrawTo(x + 2, y + 13);
  MoveTo(x + 3, y + 0); DrawTo(x + 3, y + 13);
  MoveTo(x + 4, y + 1); DrawTo(x + 4, y + 12);
  MoveTo(x + 5, y + 2); DrawTo(x + 5, y + 11);
}

void DrawHorizontalSegment(uint8_t x, uint8_t y)
{
  MoveTo(x + 2, y + 0); DrawTo(x + 11, y + 0);
  MoveTo(x + 1, y + 1); DrawTo(x + 12, y + 1);
  MoveTo(x + 0, y + 2); DrawTo(x + 13, y + 2);
  MoveTo(x + 0, y + 3); DrawTo(x + 13, y + 3);
  MoveTo(x + 1, y + 4); DrawTo(x + 12, y + 4);
  MoveTo(x + 2, y + 5); DrawTo(x + 11, y + 5);
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

void DrawSegment(uint8_t x, uint8_t y, uint8_t segment)
{
  if (segment >= 'a' && segment <= 'g')
   segment -= 'a';

  if (segment > 7)
   return;

  uint8_t o = pgm_read_byte(&segment_offsets[segment][0]);
  uint8_t ox = pgm_read_byte(&segment_offsets[segment][1]);
  uint8_t oy = pgm_read_byte(&segment_offsets[segment][2]);

  if (o == 0)
    DrawHorizontalSegment(x + ox, y + oy);
  else
    DrawVerticalSegment(x + ox, y + oy);
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

void DrawDigit(uint8_t x, uint8_t y, uint8_t digit)
{
  if (digit >= '0' && digit <= '9')
    digit -= '0';

  if (digit > 9)
    return;

  for (uint8_t i = 0; i < 8; i++) {
    uint8_t c = pgm_read_byte(&digit_segments[digit][i]);
    if (c == 0)
      break;

    DrawSegment(x, y, c);
  }
}

void DrawDot(uint8_t x, uint8_t y, uint8_t size)
{
  for (uint8_t i = 0; i < size; i++) {
    MoveTo(x, y + i);
    DrawTo(x + size - 1, y + i);
  }
}

void DrawDots()
{
  DrawDot(61, 13, 6);
  DrawDot(61, 24, 6);
}

void DrawDigits(uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4)
{
  DrawDigit(8, 5, digit1);
  DrawDigit(34, 5, digit2);
  DrawDigit(64 + 8, 5, digit3);
  DrawDigit(64 + 34, 5, digit4);
}

//-----------------------------------------------------------------------------

void setup()
{
  Wire.begin();

  ClearDisplay();
  InitDisplay();
}

//-----------------------------------------------------------------------------

uint8_t digit = 0;

void loop()
{
  ClearDisplay();
  DrawDots();
  DrawDigits(digit, digit, digit, digit);

  if (digit++ > 9)
    digit = 0;

  delay(3000);
}
