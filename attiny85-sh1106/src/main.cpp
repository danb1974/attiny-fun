/*
  Uses code from:

  Tiny Graphics Library v3 - see http://www.technoblogy.com/show?23OS
  David Johnson-Davies - www.technoblogy.com - 23rd September 2018

  Adafruit TinyRTCLib
  https://github.com/adafruit/RTClib
  
  BH1750 library
  https://github.com/PeterEmbedded/BH1750FVI

  8x8 font
  https://github.com/rene-d/fontino/
*/

#include <Arduino.h>
#include <Wire.h>

// do not set MINLUX to 0 !!!

// sensor exposed
// #define MINLUX 10
// #define MAXLUX 400
// #define MINCONTRAST 1
// #define MAXCONTRAST 127

// sensor in box, tiny hole
#define MINLUX 1
#define MAXLUX 30
#define MINCONTRAST 3
#define MAXCONTRAST 255

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

void veryFastBlink() {
  blink(10);
}

//-----------------------------------------------------------------------------

#define SH1106_ADDRESS 0x3c
#define SH1106_COMMAND 0x80
#define SH1106_DATA 0xC0
#define SH1106_COMMANDS 0x00
#define SH1106_DATAS 0x40

const uint8_t font8x8_ib8x8u[][8] PROGMEM = {
    // ---------------------------  32-127 ---------------------------
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // 0020 (space)
    {0x30, 0x78, 0x78, 0x30, 0x30, 0x00, 0x30, 0x00},  // 0021 (exclam)
    {0x6c, 0x6c, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00},  // 0022 (quotedbl)
    {0x6c, 0x6c, 0xfe, 0x6c, 0xfe, 0x6c, 0x6c, 0x00},  // 0023 (numbersign)
    {0x30, 0x7c, 0xc0, 0x78, 0x0c, 0xf8, 0x30, 0x00},  // 0024 (dollar)
    {0x00, 0xc6, 0xcc, 0x18, 0x30, 0x66, 0xc6, 0x00},  // 0025 (percent)
    {0x38, 0x6c, 0x38, 0x76, 0xdc, 0xcc, 0x76, 0x00},  // 0026 (ampersand)
    {0x60, 0x60, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00},  // 0027 (quotesingle)
    {0x18, 0x30, 0x60, 0x60, 0x60, 0x30, 0x18, 0x00},  // 0028 (parenleft)
    {0x60, 0x30, 0x18, 0x18, 0x18, 0x30, 0x60, 0x00},  // 0029 (parenright)
    {0x00, 0x66, 0x3c, 0xff, 0x3c, 0x66, 0x00, 0x00},  // 002a (asterisk)
    {0x00, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x00, 0x00},  // 002b (plus)
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x60},  // 002c (comma)
    {0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00},  // 002d (hyphen)
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00},  // 002e (period)
    {0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x80, 0x00},  // 002f (slash)
    {0x7c, 0xc6, 0xce, 0xde, 0xf6, 0xe6, 0x7c, 0x00},  // 0030 (zero)
    {0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x00},  // 0031 (one)
    {0x78, 0xcc, 0x0c, 0x38, 0x60, 0xc4, 0xfc, 0x00},  // 0032 (two)
    {0x78, 0xcc, 0x0c, 0x38, 0x0c, 0xcc, 0x78, 0x00},  // 0033 (three)
    {0x1c, 0x3c, 0x6c, 0xcc, 0xfe, 0x0c, 0x1e, 0x00},  // 0034 (four)
    {0xfc, 0xc0, 0xf8, 0x0c, 0x0c, 0xcc, 0x78, 0x00},  // 0035 (five)
    {0x38, 0x60, 0xc0, 0xf8, 0xcc, 0xcc, 0x78, 0x00},  // 0036 (six)
    {0xfc, 0xcc, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x00},  // 0037 (seven)
    {0x78, 0xcc, 0xcc, 0x78, 0xcc, 0xcc, 0x78, 0x00},  // 0038 (eight)
    {0x78, 0xcc, 0xcc, 0x7c, 0x0c, 0x18, 0x70, 0x00},  // 0039 (nine)
    {0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00},  // 003a (colon)
    {0x00, 0x30, 0x30, 0x00, 0x30, 0x30, 0x60, 0x00},  // 003b (semicolon)
    {0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x00},  // 003c (less)
    {0x00, 0x00, 0xfc, 0x00, 0x00, 0xfc, 0x00, 0x00},  // 003d (equal)
    {0x60, 0x30, 0x18, 0x0c, 0x18, 0x30, 0x60, 0x00},  // 003e (greater)
    {0x78, 0xcc, 0x0c, 0x18, 0x30, 0x00, 0x30, 0x00},  // 003f (question)
    {0x7c, 0xc6, 0xde, 0xde, 0xde, 0xc0, 0x78, 0x00},  // 0040 (at)
    {0x30, 0x78, 0xcc, 0xcc, 0xfc, 0xcc, 0xcc, 0x00},  // 0041 (A)
    {0xfc, 0x66, 0x66, 0x7c, 0x66, 0x66, 0xfc, 0x00},  // 0042 (B)
    {0x3c, 0x66, 0xc0, 0xc0, 0xc0, 0x66, 0x3c, 0x00},  // 0043 (C)
    {0xf8, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0xf8, 0x00},  // 0044 (D)
    {0xfe, 0x62, 0x68, 0x78, 0x68, 0x62, 0xfe, 0x00},  // 0045 (E)
    {0xfe, 0x62, 0x68, 0x78, 0x68, 0x60, 0xf0, 0x00},  // 0046 (F)
    {0x3c, 0x66, 0xc0, 0xc0, 0xce, 0x66, 0x3e, 0x00},  // 0047 (G)
    {0xcc, 0xcc, 0xcc, 0xfc, 0xcc, 0xcc, 0xcc, 0x00},  // 0048 (H)
    {0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00},  // 0049 (I)
    {0x1e, 0x0c, 0x0c, 0x0c, 0xcc, 0xcc, 0x78, 0x00},  // 004a (J)
    {0xe6, 0x66, 0x6c, 0x78, 0x6c, 0x66, 0xe6, 0x00},  // 004b (K)
    {0xf0, 0x60, 0x60, 0x60, 0x62, 0x66, 0xfe, 0x00},  // 004c (L)
    {0xc6, 0xee, 0xfe, 0xfe, 0xd6, 0xc6, 0xc6, 0x00},  // 004d (M)
    {0xc6, 0xe6, 0xf6, 0xde, 0xce, 0xc6, 0xc6, 0x00},  // 004e (N)
    {0x38, 0x6c, 0xc6, 0xc6, 0xc6, 0x6c, 0x38, 0x00},  // 004f (O)
    {0xfc, 0x66, 0x66, 0x7c, 0x60, 0x60, 0xf0, 0x00},  // 0050 (P)
    {0x78, 0xcc, 0xcc, 0xcc, 0xdc, 0x78, 0x1c, 0x00},  // 0051 (Q)
    {0xfc, 0x66, 0x66, 0x7c, 0x6c, 0x66, 0xe6, 0x00},  // 0052 (R)
    {0x78, 0xcc, 0xe0, 0x70, 0x1c, 0xcc, 0x78, 0x00},  // 0053 (S)
    {0xfc, 0xb4, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00},  // 0054 (T)
    {0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xfc, 0x00},  // 0055 (U)
    {0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x00},  // 0056 (V)
    {0xc6, 0xc6, 0xc6, 0xd6, 0xfe, 0xee, 0xc6, 0x00},  // 0057 (W)
    {0xc6, 0xc6, 0x6c, 0x38, 0x38, 0x6c, 0xc6, 0x00},  // 0058 (X)
    {0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x30, 0x78, 0x00},  // 0059 (Y)
    {0xfe, 0xc6, 0x8c, 0x18, 0x32, 0x66, 0xfe, 0x00},  // 005a (Z)
    // {0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x78, 0x00},  // 005b (bracketleft)
    // {0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x02, 0x00},  // 005c (backslash)
    // {0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00},  // 005d (bracketright)
    // {0x10, 0x38, 0x6c, 0xc6, 0x00, 0x00, 0x00, 0x00},  // 005e (asciicircum)
    // {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff},  // 005f (underscore)
    // {0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},  // 0060 (grave)
    // {0x00, 0x00, 0x78, 0x0c, 0x7c, 0xcc, 0x76, 0x00},  // 0061 (a)
    // {0xe0, 0x60, 0x60, 0x7c, 0x66, 0x66, 0xdc, 0x00},  // 0062 (b)
    // {0x00, 0x00, 0x78, 0xcc, 0xc0, 0xcc, 0x78, 0x00},  // 0063 (c)
    // {0x1c, 0x0c, 0x0c, 0x7c, 0xcc, 0xcc, 0x76, 0x00},  // 0064 (d)
    // {0x00, 0x00, 0x78, 0xcc, 0xfc, 0xc0, 0x78, 0x00},  // 0065 (e)
    // {0x38, 0x6c, 0x60, 0xf0, 0x60, 0x60, 0xf0, 0x00},  // 0066 (f)
    // {0x00, 0x00, 0x76, 0xcc, 0xcc, 0x7c, 0x0c, 0xf8},  // 0067 (g)
    // {0xe0, 0x60, 0x6c, 0x76, 0x66, 0x66, 0xe6, 0x00},  // 0068 (h)
    // {0x30, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00},  // 0069 (i)
    // {0x0c, 0x00, 0x0c, 0x0c, 0x0c, 0xcc, 0xcc, 0x78},  // 006a (j)
    // {0xe0, 0x60, 0x66, 0x6c, 0x78, 0x6c, 0xe6, 0x00},  // 006b (k)
    // {0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00},  // 006c (l)
    // {0x00, 0x00, 0xcc, 0xfe, 0xfe, 0xd6, 0xc6, 0x00},  // 006d (m)
    // {0x00, 0x00, 0xf8, 0xcc, 0xcc, 0xcc, 0xcc, 0x00},  // 006e (n)
    // {0x00, 0x00, 0x78, 0xcc, 0xcc, 0xcc, 0x78, 0x00},  // 006f (o)
    // {0x00, 0x00, 0xdc, 0x66, 0x66, 0x7c, 0x60, 0xf0},  // 0070 (p)
    // {0x00, 0x00, 0x76, 0xcc, 0xcc, 0x7c, 0x0c, 0x1e},  // 0071 (q)
    // {0x00, 0x00, 0xdc, 0x76, 0x66, 0x60, 0xf0, 0x00},  // 0072 (r)
    // {0x00, 0x00, 0x7c, 0xc0, 0x78, 0x0c, 0xf8, 0x00},  // 0073 (s)
    // {0x10, 0x30, 0x7c, 0x30, 0x30, 0x34, 0x18, 0x00},  // 0074 (t)
    // {0x00, 0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0x76, 0x00},  // 0075 (u)
    // {0x00, 0x00, 0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x00},  // 0076 (v)
    // {0x00, 0x00, 0xc6, 0xd6, 0xfe, 0xfe, 0x6c, 0x00},  // 0077 (w)
    // {0x00, 0x00, 0xc6, 0x6c, 0x38, 0x6c, 0xc6, 0x00},  // 0078 (x)
    // {0x00, 0x00, 0xcc, 0xcc, 0xcc, 0x7c, 0x0c, 0xf8},  // 0079 (y)
    // {0x00, 0x00, 0xfc, 0x98, 0x30, 0x64, 0xfc, 0x00},  // 007a (z)
    // {0x1c, 0x30, 0x30, 0xe0, 0x30, 0x30, 0x1c, 0x00},  // 007b (braceleft)
    // {0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},  // 007c (bar)
    // {0xe0, 0x30, 0x30, 0x1c, 0x30, 0x30, 0xe0, 0x00},  // 007d (braceright)
    // {0x76, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // 007e (asciitilde)
    // {0x00, 0x10, 0x38, 0x6c, 0xc6, 0xc6, 0xfe, 0x00},  // 007f (uni007F)
};

void DisplaySingleCommand(uint8_t data)
{
  Wire.write(SH1106_COMMAND);
  Wire.write(data);
}

void DisplayInit()
{
  Wire.beginTransmission(SH1106_ADDRESS);
  Wire.write(SH1106_COMMANDS);
  Wire.write(0xA1); // Flip horizontal
  Wire.write(0xC8); // Flip vertical
  Wire.write(0xAF); // Display on
  Wire.endTransmission();
}

void DisplayClear(uint8_t fill = 0x0)
{
  for (uint8_t p = 0; p < 8; p++)
  {
    Wire.beginTransmission(SH1106_ADDRESS);
    DisplaySingleCommand(0x00 | 2); // Column low nibble
    DisplaySingleCommand(0x10 | 0); // Column high nibble
    DisplaySingleCommand(0xB0 | p); // Page
    Wire.endTransmission();

    // send 128 zeros in batches (column autoincrements with each write)
    for (uint8_t q = 0; q < 16; q++)
    {
      Wire.beginTransmission(SH1106_ADDRESS);
      Wire.write(SH1106_DATAS);

      for (uint8_t i = 0; i < 8; i++)
        Wire.write(fill);

      Wire.endTransmission();
    }
  }
}

void DisplayPlotPoint(uint8_t x, uint8_t y, bool clear = false)
{
  x += 2;       // driver has 132 rows but uses 2-129 not 0-127
  y = 63 - y;   // start bottom left

  Wire.beginTransmission(SH1106_ADDRESS);
  DisplaySingleCommand(0x00 | (x & 0x0F));        // Column low nibble
  DisplaySingleCommand(0x10 | (x >> 4));          // Column high nibble
  DisplaySingleCommand(0xB0 | (y >> 3));          // Page
  DisplaySingleCommand(0xE0);                     // Enter read modify write
  Wire.write(SH1106_DATA);
  Wire.endTransmission();

  Wire.requestFrom(SH1106_ADDRESS, 2);
  Wire.read();                                    // Dummy read (see datasheet)
  uint8_t j = Wire.read();

  Wire.beginTransmission(SH1106_ADDRESS);
  Wire.write(SH1106_DATA);
  uint8_t mask = 1 << (y & 0x07);
  if (clear)
    Wire.write(~mask & j);
  else
    Wire.write(mask | j);
  DisplaySingleCommand(0xEE);                     // Cancel read modify write
  Wire.endTransmission();
}

void DisplayDrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool clear = false)
{
  int8_t sx, sy, e2, err;

  uint8_t dx = abs(x2 - x1);
  uint8_t dy = abs(y2 - y1);

  sx = (x1 < x2) ? 1 : -1;
  sy = (y1 < y2) ? 1 : -1;

  err = dx - dy;
  for (;;)
  {
    DisplayPlotPoint(x1, y1, clear);

    if (x1 == x2 && y1 == y2)
      return;

    e2 = err << 1;

    if (e2 > -dy)
    {
      err -= dy;
      x1 += sx;
    }
    else if (e2 < dx)
    {
      err += dx;
      y1 += sy;
    }
  }
}

void DisplayBitmapAt(uint8_t row, uint8_t col, uint8_t *bitmap)
{
  // since display is flipped and we want to have 0,0 top left, some things are reversed

  // flip 90 degrees
  uint8_t buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t mask = 0x01;
  for (uint8_t r = 0; r < 8; r++) {
    uint8_t byte = bitmap[r];

    for (uint8_t c = 0; c < 8; c++) {
      if (byte & 0x80) {
        buffer[c] |= mask;
      }
      byte <<= 1;
    }

    mask <<= 1;
  }

  col = (col << 3) + 2; // SSD1306 has offset 0, SH1106 has offset 2

  Wire.beginTransmission(SH1106_ADDRESS);
  DisplaySingleCommand(0x00 | (col & 0x0F));  // Column low nibble
  DisplaySingleCommand(0x10 | (col >> 4));    // Column high nibble
  DisplaySingleCommand(0xB0 | row);           // Page
  Wire.endTransmission();

  Wire.beginTransmission(SH1106_ADDRESS);
  Wire.write(SH1106_DATAS);
  for (uint8_t i = 0; i < 8; i++) {
    Wire.write(buffer[i]);
  }
  Wire.endTransmission();
}

void printCharAt(uint8_t row, uint8_t col, uint8_t c) {
  uint8_t bitmap[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  if (c != ' ') {
    // lowercase only due to memory constraints
    if (c >= 'a' && c <= 'z') {
      c -= 'a' - 'A';
    }
    if (c < ' ' || c > 'Z') {
      c = ' ';
    }

    for (uint8_t i = 0; i < 8; i++) {
      bitmap[i] = pgm_read_byte(&font8x8_ib8x8u[c - 0x20][i]); 
    }
  }

  DisplayBitmapAt(row, col, bitmap);
}

void printStrAt(uint8_t row, uint8_t col, const char *s) {
  while (*s != 0) {
    printCharAt(row, col++, *s++);

    if (col > 15) {
      col = 0;
      row++;
    }
    
    if (row > 7) {
      row = 0;
    }
  }
}

void DisplaySetContrast(uint8_t contrast) {
  Wire.beginTransmission(SH1106_ADDRESS);
  Wire.write(SH1106_COMMANDS);
  Wire.write(0x81);
  Wire.write(contrast);
  Wire.endTransmission();
}

//-----------------------------------------------------------------------------

void DrawVerticalSegment(uint8_t x, uint8_t y, bool clear = false)
{
  uint8_t lines[][2] = {
    {(uint8_t)(y + 2), (uint8_t)(y + 11)},
    {(uint8_t)(y + 1), (uint8_t)(y + 12)},
    {(uint8_t)(y + 0), (uint8_t)(y + 13)},
    {(uint8_t)(y + 0), (uint8_t)(y + 13)},
    {(uint8_t)(y + 1), (uint8_t)(y + 12)},
    {(uint8_t)(y + 2), (uint8_t)(y + 11)},
  };

  for (uint8_t i = 0; i < 6; i++)
    DisplayDrawLine(x + i, lines[i][0], x + i, lines[i][1], clear);
}

void DrawHorizontalSegment(uint8_t x, uint8_t y, bool clear = false)
{
  uint8_t lines[][2] = {
    {(uint8_t)(x + 2), (uint8_t)(x + 11)},
    {(uint8_t)(x + 1), (uint8_t)(x + 12)},
    {(uint8_t)(x + 0), (uint8_t)(x + 13)},
    {(uint8_t)(x + 0), (uint8_t)(x + 13)},
    {(uint8_t)(x + 1), (uint8_t)(x + 12)},
    {(uint8_t)(x + 2), (uint8_t)(x + 11)},
  };

  for (uint8_t i = 0; i < 6; i++)
    DisplayDrawLine(lines[i][0], y + i, lines[i][1], y + i, clear);
}

// orientation, x, y
const uint8_t segment_offsets[7][3] PROGMEM = {
  { 0, 4,  32 },
  { 1, 16, 20 },
  { 1, 16, 4  },
  { 0, 4,  0  },
  { 1, 0,  4  },
  { 1, 0,  20 },
  { 0, 4,  16 }
};

void DrawSegment(uint8_t x, uint8_t y, uint8_t segment, bool clear = false)
{
  // support both letters and integers
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

const uint8_t digit_segments[10][8] PROGMEM = {
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
    DisplayDrawLine(x, y + i, x + size - 1, y + i, clear);
}

void DrawDots(bool clear = false)
{
  // 6x6
  // DrawDot(61, 8, 6, clear);
  // DrawDot(61, 24, 6, clear);

  // 4x4
  DrawDot(61, 5 + 9, 4, clear);
  DrawDot(61, 5 + 25, 4, clear);
}

void DrawDigitPos(uint8_t pos, uint8_t digit, bool clear = false)
{
  const uint8_t digitXPos[] = {8,  34, 72, 98};
  const uint8_t digitYPos[] = {5,  5,  5,  5};

  if (pos > 3)
    pos = 3;

  DrawDigit(digitXPos[pos], digitYPos[pos], digit, clear);
}

// void DrawDigits(uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4)
// {
//   DrawDigitPos(0, digit1);
//   DrawDigitPos(1, digit2);
//   DrawDigitPos(2, digit3);
//   DrawDigitPos(3, digit4);
// }

//-----------------------------------------------------------------------------

uint8_t doubleDigitToUint8(const char *p)
{
  uint8_t number = 0;

  for (uint8_t i = 0; i < 2; i++) {
    number *= 10;
    if ('1' <= *p && *p <= '9') {
      number += *p - '0';
    }
    p++;
  }

  return number;
}

struct DateTime {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
};

struct DateTime compilerStampToDateTime(const char *date, const char *time)
{
  struct DateTime dt;

  //                       0123456789A           01234567
  // sample input: date = "Jan 01 2023", time = "12:34:56"
  dt.year = doubleDigitToUint8(date + 7) * 100 + doubleDigitToUint8(date + 9);
  // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
  switch (date[0])
  {
  case 'J':
    if (date[1] == 'a')
      dt.month = 1;
    else 
      dt.month = date[2] == 'n' ? 6 : 7;
    break;
  case 'F':
    dt.month = 2;
    break;
  case 'A':
    dt.month = date[2] == 'r' ? 4 : 8;
    break;
  case 'M':
    dt.month = date[2] == 'r' ? 3 : 5;
    break;
  case 'S':
    dt.month = 9;
    break;
  case 'O':
    dt.month = 10;
    break;
  case 'N':
    dt.month = 11;
    break;
  case 'D':
    dt.month = 12;
    break;
  default:
    // oh no
    dt.month = 1;
  }
  dt.day = doubleDigitToUint8(date + 4);

  dt.hour = doubleDigitToUint8(time);
  dt.minute = doubleDigitToUint8(time + 3);
  dt.second = doubleDigitToUint8(time + 6);

  return dt;
}

//-----------------------------------------------------------------------------

#define DS1307_ADDRESS 0x68

static uint8_t bcd2bin(uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd(uint8_t val) { return val + 6 * (val / 10); }

uint8_t RtcIsRunning()
{
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 1);
  uint8_t ss = Wire.read();
  return !(ss >> 7);
}

void RtcAdjust(const struct DateTime &dt)
{
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(0);
  Wire.write(0x80); // stop clock, will write seconds later
  Wire.write(bin2bcd(dt.minute));
  Wire.write(bin2bcd(dt.hour));
  Wire.write(bin2bcd(0)); // DOW
  Wire.write(bin2bcd(dt.day));
  Wire.write(bin2bcd(dt.month));
  Wire.write(bin2bcd((dt.year - 2000) & 0xff));
  //Wire.write(0);
  Wire.endTransmission();

  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(0);
  Wire.write(bin2bcd(dt.second & 0x7f)); // update seconds and start clock
  Wire.endTransmission();
}

struct DateTime RtcNow()
{
  struct DateTime dt;

  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 7);
  dt.second = bcd2bin(Wire.read() & 0x7F);
  dt.minute = bcd2bin(Wire.read());
  dt.hour = bcd2bin(Wire.read());
  Wire.read();
  dt.day = bcd2bin(Wire.read());
  dt.month = bcd2bin(Wire.read());
  dt.year = bcd2bin(Wire.read()) + 2000;

  return dt;
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

void sprintfUint8(char *buf, uint8_t number, uint8_t padLen = 0, char padChar = ' ') {
  char tmp[4];

  char *ptr = tmp;
  uint8_t divisor = 100;
  uint8_t zeroes = 0;
  bool leading = true;

  while (divisor > 0) {
    uint8_t digit = number / divisor;

    if (digit != 0)
      leading = false;

    if (leading && divisor > 1 && digit == 0) {
      zeroes++;
      *ptr++ = padChar;
    } else {
      *ptr++ = digit + '0';
    }

    number -= digit * divisor;
    divisor /= 10;
  }
  *ptr = 0;

  // we now have the padded number to three characters and the number of leading zeroes
  padLen = max(padLen, 3 - zeroes);
  strcpy(buf, tmp + (3 - padLen));
}

//-----------------------------------------------------------------------------

volatile uint8_t presses = 0;
volatile bool pressed = false;

ISR(PCINT0_vect)
{
  static unsigned long lastTimeinterrupt = 0;
  unsigned long timeInterrupt = millis();

  pressed = digitalRead(PIN3) == LOW;
  digitalWrite(PIN1, pressed ? HIGH : LOW);

  if (pressed && timeInterrupt - lastTimeinterrupt > 10) {
    presses++;
  }

  lastTimeinterrupt = timeInterrupt;
}

//-----------------------------------------------------------------------------

uint8_t contrast = MAXCONTRAST;
uint8_t desiredContrast = contrast;

void setup()
{
  pinMode(PIN1, OUTPUT);
  slowBlink();

  // button generates interrupt on change
  cli();
  PCMSK |= (1 << PCINT3);
  GIMSK |= (1 << PCIE);
  pinMode(PIN3, INPUT_PULLUP);
  sei();    
  fastBlink();

  Wire.begin();
  fastBlink();
  
  if (!RtcIsRunning()) {
    RtcAdjust(compilerStampToDateTime(__DATE__, __TIME__));
    slowBlink();
  }

  DisplayClear();
  DisplayInit();
  DisplaySetContrast(contrast);
  fastBlink();

  LightInit();
  fastBlink();

  // display test
  DisplayClear(0xff);
  delay(1000);
  DisplayClear();
  delay(1000);

  // before being "just as clock" show things
  char buffer[17];

  printStrAt(2, 2, "Hello family");

  DateTime now = RtcNow();
  // time
  sprintfUint8(buffer + 0, now.hour, 2, ' ');
  buffer[2] = ':';
  sprintfUint8(buffer + 3, now.minute, 2, '0');
  printStrAt(4, 5, buffer);
  // date
  sprintfUint8(buffer + 0, now.day, 2, ' ');
  buffer[2] = '.';
  sprintfUint8(buffer + 3, now.month, 2, '0');
  buffer[5] = '.';
  sprintfUint8(buffer + 6, now.year / 100, 2, '0');
  sprintfUint8(buffer + 8, now.year % 100, 2, '0');
  printStrAt(5, 3, buffer);

  uint16_t light = LightGetIntensity();
  sprintfUint8(buffer + 0, light / 10000, 1); light %= 10000;
  sprintfUint8(buffer + 1, light / 100, 2, '0');
  sprintfUint8(buffer + 3, light % 100, 2, '0');
  printStrAt(7, 5, buffer);

  delay(10000);
  DisplayClear();
}

//-----------------------------------------------------------------------------

uint8_t oldDigits[4] = {0xff, 0xff, 0xff, 0xff};
uint8_t digits[4] = {0, 0, 0, 0};
bool dots = true;

enum State {
  Normal = 1,
  Hour = 2,
  Minute = 3
};
State state = Normal;

uint8_t stateCounter = 0;
uint8_t oldPresses = 0;
uint8_t setHour;
uint8_t setMinute;

void loop()
{
  delay(1000);

  DateTime now = RtcNow();
  uint8_t hour = now.hour;
  uint8_t minute = now.minute;

  digits[0] = hour / 10;
  digits[1] = hour % 10;
  digits[2] = minute / 10;
  digits[3] = minute % 10;

  if (state == Normal) {
   if (pressed) {
      stateCounter++;
      if (stateCounter > 3) {
        state = Hour;
        stateCounter = 0;

        presses = 1;
        oldPresses = 0;

        setHour = now.hour;
        setMinute = now.minute;

        DisplayClear();
        return;
      }
    } else {
      stateCounter = 0;
    }

    for (uint8_t i = 0; i < 4; i++)
    {
      if (digits[i] != oldDigits[i])
      {
        // clear old digit
        if (oldDigits[i] != 0xff)
          DrawDigitPos(i, oldDigits[i], true);

        // do not draw first zero
        if (i > 0 || digits[i] != 0)
          DrawDigitPos(i, digits[i], false);

        oldDigits[i] = digits[i];
      }
    }

    DrawDots(dots);
    dots = !dots;

    uint16_t light = LightGetIntensity();
    if (light < MINLUX) {
      desiredContrast = MINCONTRAST;
    } else if (light > MAXLUX) {
      desiredContrast = MAXCONTRAST;
    } else {
      uint16_t offset = (light - MINLUX) * (MAXCONTRAST - MINCONTRAST) / (MAXLUX - MINLUX);
      desiredContrast = MINCONTRAST + offset;
    }
        
    // just to be sure
    if (contrast < MINCONTRAST)
      contrast = MINCONTRAST;
    if (contrast > MAXCONTRAST)
      contrast = MAXCONTRAST;

    uint8_t diff = abs(contrast - desiredContrast);
    if (diff > 1) {
      uint8_t step = max(diff / 32, 2);
      contrast += (contrast < desiredContrast) ? step : -step;
      DisplaySetContrast(contrast);
      veryFastBlink();
    }
  }

  if (state == Hour) {
    if (presses > oldPresses) {
      oldPresses = presses;
      stateCounter = 0;

      if (presses > 1) setHour++;
      if (setHour > 23) setHour = 0;

      DrawDigitPos(0, setHour / 10);
      DrawDigitPos(1, setHour % 10);
    } else {
      stateCounter++;
      if (stateCounter > 5) {
        state = Minute;
        stateCounter = 0;

        presses = 1;
        oldPresses = 0;

        DisplayClear();
        return;
      }
    }

    return;
  }

  if (state == Minute) {
    if (presses > oldPresses) {
      oldPresses = presses;
      stateCounter = 0;

      if (presses > 1) setMinute++;
      if (setMinute > 59) setMinute = 0;

      DrawDigitPos(2, setMinute / 10);
      DrawDigitPos(3, setMinute % 10);
    } else {
      stateCounter++;
      if (stateCounter > 5) {
        state = Normal;
        stateCounter = 0;

        oldDigits[0] = 0xff;
        oldDigits[1] = 0xff;
        oldDigits[2] = 0xff;
        oldDigits[3] = 0xff;

        // save new time
        DateTime newDt;
        newDt.hour = setHour;
        newDt.minute = setMinute;
        newDt.second = 0;
        newDt.year = now.year;
        newDt.month = now.month;
        newDt.day = now.day;
        RtcAdjust(newDt);

        DisplayClear();
        return;
      }
    }

    return;
  }

  state = Normal;
}
