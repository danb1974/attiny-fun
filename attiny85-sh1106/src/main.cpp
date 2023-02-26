#include <Arduino.h>
//#include <TinyWireM.h>
#include <SH1106mini.h>
#include <TinyRTClib.h>

SH1106mini display(0x3c);
RTC_DS1307 RTC;

uint8_t bmap[8] =
{ 
  B00111100,
  B01111110,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B01111110,
  B00111100
};

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

void setup() {
  TinyWireM.begin();

  RTC.begin();

  display.init();
  display.setContrast(255);
  display.flipScreenVertically();
  display.clear();
}

void loop() {
  display.drawBitmap(4, 2, bmap);
  delay(5000);
  
  display.printAt(5, "Hello", LEFT);

  DateTime now = RTC.now();
  unsigned char hh = now.hour();
  unsigned char mm = now.minute();
  if (hh > 23) hh = 0;
  if (mm > 59) mm = 0;

  char hhmm[6] = "--:--";
  sprintf_uint8(hhmm + 0, hh, true);
  sprintf_uint8(hhmm + 3, mm, true);

  display.printAt(6, hhmm, LEFT);
  
  delay(10000);
}
