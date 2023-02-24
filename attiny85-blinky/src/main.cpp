#include <Arduino.h>

void setup() {
  pinMode(1, OUTPUT);
}

static short count = 15;
static char buf[4];

void sprint_uint8(char *buf, uint8_t number, bool zeroPad = false) {
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

void loop() {
  sprint_uint8(buf, count++, true);

  digitalWrite(1, HIGH);
  delay(1000);
  digitalWrite(1, LOW);
  delay(1000);
}
