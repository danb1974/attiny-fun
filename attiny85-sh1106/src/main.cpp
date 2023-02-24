/* Tiny Graphics Library v3 - see http://www.technoblogy.com/show?23OS

   David Johnson-Davies - www.technoblogy.com - 23rd September 2018
   ATtiny85 @ 8 MHz (internal oscillator; BOD disabled)
   
   CC BY 4.0
   Licensed under a Creative Commons Attribution 4.0 International license: 
   http://creativecommons.org/licenses/by/4.0/

   Modified to directly call TinywireM and use PIN constants
   Temperature sensor code removed, just testing the display
*/

#include <Arduino.h>
#include <TinyWireM.h>

// Pins
int const sda = PIN0;
int const scl = PIN2;

// Constants
int const address = 0x3c; // display I2C address
int const commands = 0x00;
int const onecommand = 0x80;
int const data = 0x40;
int const onedata = 0xC0;

// OLED display **********************************************

// Character set for text - stored in program memory
const uint8_t CharMap[96][6] PROGMEM = {
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, 
{ 0x00, 0x00, 0x5F, 0x00, 0x00, 0x00 }, 
{ 0x00, 0x07, 0x00, 0x07, 0x00, 0x00 }, 
{ 0x14, 0x7F, 0x14, 0x7F, 0x14, 0x00 }, 
{ 0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x00 }, 
{ 0x23, 0x13, 0x08, 0x64, 0x62, 0x00 }, 
{ 0x36, 0x49, 0x56, 0x20, 0x50, 0x00 }, 
{ 0x00, 0x08, 0x07, 0x03, 0x00, 0x00 }, 
{ 0x00, 0x1C, 0x22, 0x41, 0x00, 0x00 }, 
{ 0x00, 0x41, 0x22, 0x1C, 0x00, 0x00 }, 
{ 0x2A, 0x1C, 0x7F, 0x1C, 0x2A, 0x00 }, 
{ 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00 }, 
{ 0x00, 0x80, 0x70, 0x30, 0x00, 0x00 }, 
{ 0x08, 0x08, 0x08, 0x08, 0x08, 0x00 }, 
{ 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 }, 
{ 0x20, 0x10, 0x08, 0x04, 0x02, 0x00 }, 
{ 0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00 }, 
{ 0x00, 0x42, 0x7F, 0x40, 0x00, 0x00 }, 
{ 0x72, 0x49, 0x49, 0x49, 0x46, 0x00 }, 
{ 0x21, 0x41, 0x49, 0x4D, 0x33, 0x00 }, 
{ 0x18, 0x14, 0x12, 0x7F, 0x10, 0x00 }, 
{ 0x27, 0x45, 0x45, 0x45, 0x39, 0x00 }, 
{ 0x3C, 0x4A, 0x49, 0x49, 0x31, 0x00 }, 
{ 0x41, 0x21, 0x11, 0x09, 0x07, 0x00 }, 
{ 0x36, 0x49, 0x49, 0x49, 0x36, 0x00 }, 
{ 0x46, 0x49, 0x49, 0x29, 0x1E, 0x00 }, 
{ 0x00, 0x00, 0x14, 0x00, 0x00, 0x00 }, 
{ 0x00, 0x40, 0x34, 0x00, 0x00, 0x00 }, 
{ 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 }, 
{ 0x14, 0x14, 0x14, 0x14, 0x14, 0x00 }, 
{ 0x00, 0x41, 0x22, 0x14, 0x08, 0x00 }, 
{ 0x02, 0x01, 0x59, 0x09, 0x06, 0x00 }, 
{ 0x3E, 0x41, 0x5D, 0x59, 0x4E, 0x00 }, 
{ 0x7C, 0x12, 0x11, 0x12, 0x7C, 0x00 }, 
{ 0x7F, 0x49, 0x49, 0x49, 0x36, 0x00 }, 
{ 0x3E, 0x41, 0x41, 0x41, 0x22, 0x00 }, 
{ 0x7F, 0x41, 0x41, 0x41, 0x3E, 0x00 }, 
{ 0x7F, 0x49, 0x49, 0x49, 0x41, 0x00 }, 
{ 0x7F, 0x09, 0x09, 0x09, 0x01, 0x00 }, 
{ 0x3E, 0x41, 0x41, 0x51, 0x73, 0x00 }, 
{ 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00 }, 
{ 0x00, 0x41, 0x7F, 0x41, 0x00, 0x00 }, 
{ 0x20, 0x40, 0x41, 0x3F, 0x01, 0x00 }, 
{ 0x7F, 0x08, 0x14, 0x22, 0x41, 0x00 }, 
{ 0x7F, 0x40, 0x40, 0x40, 0x40, 0x00 }, 
{ 0x7F, 0x02, 0x1C, 0x02, 0x7F, 0x00 }, 
{ 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00 }, 
{ 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00 }, 
{ 0x7F, 0x09, 0x09, 0x09, 0x06, 0x00 }, 
{ 0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00 }, 
{ 0x7F, 0x09, 0x19, 0x29, 0x46, 0x00 }, 
{ 0x26, 0x49, 0x49, 0x49, 0x32, 0x00 }, 
{ 0x03, 0x01, 0x7F, 0x01, 0x03, 0x00 }, 
{ 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00 }, 
{ 0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00 }, 
{ 0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00 }, 
{ 0x63, 0x14, 0x08, 0x14, 0x63, 0x00 }, 
{ 0x03, 0x04, 0x78, 0x04, 0x03, 0x00 }, 
{ 0x61, 0x59, 0x49, 0x4D, 0x43, 0x00 }, 
{ 0x00, 0x7F, 0x41, 0x41, 0x41, 0x00 }, 
{ 0x02, 0x04, 0x08, 0x10, 0x20, 0x00 }, 
{ 0x00, 0x41, 0x41, 0x41, 0x7F, 0x00 }, 
{ 0x04, 0x02, 0x01, 0x02, 0x04, 0x00 }, 
{ 0x40, 0x40, 0x40, 0x40, 0x40, 0x00 }, 
{ 0x00, 0x03, 0x07, 0x08, 0x00, 0x00 }, 
{ 0x20, 0x54, 0x54, 0x78, 0x40, 0x00 }, 
{ 0x7F, 0x28, 0x44, 0x44, 0x38, 0x00 }, 
{ 0x38, 0x44, 0x44, 0x44, 0x28, 0x00 }, 
{ 0x38, 0x44, 0x44, 0x28, 0x7F, 0x00 }, 
{ 0x38, 0x54, 0x54, 0x54, 0x18, 0x00 }, 
{ 0x00, 0x08, 0x7E, 0x09, 0x02, 0x00 }, 
{ 0x18, 0xA4, 0xA4, 0x9C, 0x78, 0x00 }, 
{ 0x7F, 0x08, 0x04, 0x04, 0x78, 0x00 }, 
{ 0x00, 0x44, 0x7D, 0x40, 0x00, 0x00 }, 
{ 0x20, 0x40, 0x40, 0x3D, 0x00, 0x00 }, 
{ 0x7F, 0x10, 0x28, 0x44, 0x00, 0x00 }, 
{ 0x00, 0x41, 0x7F, 0x40, 0x00, 0x00 }, 
{ 0x7C, 0x04, 0x78, 0x04, 0x78, 0x00 }, 
{ 0x7C, 0x08, 0x04, 0x04, 0x78, 0x00 }, 
{ 0x38, 0x44, 0x44, 0x44, 0x38, 0x00 }, 
{ 0xFC, 0x18, 0x24, 0x24, 0x18, 0x00 }, 
{ 0x18, 0x24, 0x24, 0x18, 0xFC, 0x00 }, 
{ 0x7C, 0x08, 0x04, 0x04, 0x08, 0x00 }, 
{ 0x48, 0x54, 0x54, 0x54, 0x24, 0x00 }, 
{ 0x04, 0x04, 0x3F, 0x44, 0x24, 0x00 }, 
{ 0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00 }, 
{ 0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00 }, 
{ 0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00 }, 
{ 0x44, 0x28, 0x10, 0x28, 0x44, 0x00 }, 
{ 0x4C, 0x90, 0x90, 0x90, 0x7C, 0x00 }, 
{ 0x44, 0x64, 0x54, 0x4C, 0x44, 0x00 }, 
{ 0x00, 0x08, 0x36, 0x41, 0x00, 0x00 }, 
{ 0x00, 0x00, 0x77, 0x00, 0x00, 0x00 }, 
{ 0x00, 0x41, 0x36, 0x08, 0x00, 0x00 }, 
{ 0x00, 0x06, 0x09, 0x06, 0x00, 0x00 },  // degree symbol = '~'
{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00 }

};

// Current plot position
int x0;
int y0;

// Write a single command
void Single (uint8_t x) {
  TinyWireM.write(onecommand);
  TinyWireM.write(x);
}
  
void InitDisplay () {
  TinyWireM.beginTransmission(address);
  TinyWireM.write(commands);
  TinyWireM.write(0xA1);  // Flip horizontal
  TinyWireM.write(0xAF);  // Display on
  TinyWireM.endTransmission();
}

void ClearDisplay () {
  for (int p = 0 ; p < 8; p++) {
    TinyWireM.beginTransmission(address);
    Single(0xB0 + p);
    TinyWireM.endTransmission();
    for (int q = 0 ; q < 8; q++) {
      TinyWireM.beginTransmission(address);
      TinyWireM.write(data);
      for (int i = 0 ; i < 20; i++) TinyWireM.write(0);
      TinyWireM.endTransmission();
    }
  }
}

// Plot point x,y into buffer if in current slice
void PlotPoint (int x, int y) {
  TinyWireM.beginTransmission(address);
  Single(0x00 + ((x + 2) & 0x0F));        // Column low nibble
  Single(0x10 + ((x + 2)>>4));            // Column high nibble
  Single(0xB0 + (y >> 3));                // Page
  Single(0xE0);                           // Read modify write
  TinyWireM.write(onedata);
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(address, 2);
  TinyWireM.read();                            // Dummy read
  int j = TinyWireM.read();
  TinyWireM.beginTransmission(address);
  TinyWireM.write(onedata);
  TinyWireM.write((1<<(y & 0x07)) | j);
  Single(0xEE);                           // Cancel read modify write
  TinyWireM.endTransmission();
}

// Move current plot position to x,y
void MoveTo (int x, int y) {
  x0 = x;
  y0 = y;
}

// Draw a line to x,y
void DrawTo (int x, int y) {
  int sx, sy, e2, err;
  int dx = abs(x - x0);
  int dy = abs(y - y0);
  if (x0 < x) sx = 1; else sx = -1;
  if (y0 < y) sy = 1; else sy = -1;
  err = dx - dy;
  for (;;) {
    PlotPoint(x0, y0);
    if (x0==x && y0==y) return;
    e2 = err<<1;
    if (e2 > -dy) { err = err - dy; x0 = x0 + sx; }
    if (e2 < dx) { err = err + dx; y0 = y0 + sy; }
  }
}

uint8_t ReverseByte (uint8_t x) {
  x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
  x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
  x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
  return x;    
}

// Plot an ASCII character with bottom left corner at x,y
void PlotChar (int c, int x, int y) {
  int h = y & 0x07;
  for (int p = 0; p < 2; p++) {
    TinyWireM.beginTransmission(address);
    Single(0xB0 + (y >> 3) + p);          // Page
    for (int col=0; col<6; col++) {
      Single(0x00 + ((x+2+col) & 0x0F));  // Column low nibble
      Single(0x10 + ((x+2+col)>>4));      // Column high nibble
      Single(0xE0);                       // Read modify write
      TinyWireM.write(onedata);
      TinyWireM.endTransmission();
      TinyWireM.requestFrom(address, 2);
      TinyWireM.read();                        // Dummy read
      int j = TinyWireM.read();
      TinyWireM.beginTransmission(address);
      TinyWireM.write(onedata);
      int bits = ReverseByte(pgm_read_byte(&CharMap[c-32][col]));
      TinyWireM.write((bits<<h)>>(p<<3) | j);
      Single(0xEE);                       // Cancel read modify write
    }
    TinyWireM.endTransmission();
  }
}

// Plot text starting at the current plot position
void PlotText(PGM_P s) {
  int p = (int)s;
  while (1) {
    char c = pgm_read_byte(p++);
    if (c == 0) return;
    PlotChar(c, x0, y0);
    x0 = x0 + 6;
  }
}

// Setup **********************************************

void setup() {
  TinyWireM.begin();
  ClearDisplay();
  InitDisplay();
}

const int Now = 1547;                   // To set the time; eg 15:47
unsigned long StartMins = (unsigned long)((Now/100)*60 + (Now%100));

void loop () {
  unsigned int SampleNo = StartMins/15;
  // Plot temperature graph
  int x1 = 16, y1 = 11;
  int yscale = 2;                         // Points per degree
  MoveTo(26, 56); PlotText(PSTR("Temperature ~C"));
  // Horizontal axis
  MoveTo(x1, y1); DrawTo(x1+96, y1);
  for (int i=0; i<=24; i=i+4) {
    int mark = x1+i*4;
    MoveTo(mark, y1); DrawTo(mark, y1-2);
    int tens = i/10;
    if (tens != 0) {
      PlotChar(tens+'0', mark-6, y1-12);
      PlotChar(i%10+'0', mark, y1-12);
    } else PlotChar(i%10+'0', mark-3, y1-12);
  }
  // Vertical axis
  MoveTo(x1, y1); DrawTo(x1, y1+50);
  for (int i=5; i<=25; i=i+5) {
    int mark = y1+i*yscale-10;
    MoveTo(x1, mark); DrawTo(x1-2, mark);
    int tens = i/10;
    if (tens != 0) PlotChar(tens+'0', x1-15, mark-3);
    PlotChar(i%10+'0', x1-9, mark-3);
  }
  for (;;) {
    // Now start plotting the temperature every 15 mins
    while ((unsigned long) ((StartMins + millis()/60000)/15)%96 == SampleNo);
    // Time to take a new reading
    SampleNo = (SampleNo+1)%96;
    int Temperature = 25;  // In half degrees // Just testing the display, no sensor
    PlotPoint(SampleNo+x1, Temperature-10+y1);
  }
}