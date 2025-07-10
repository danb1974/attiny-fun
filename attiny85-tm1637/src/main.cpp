#include <Arduino.h>
#include <TM1637Display.h>

#define CLK 2
#define DIO 0

TM1637Display display(CLK, DIO);

void setup() {
  display.setBrightness(0x07); // 0x0f is maximum
}

void loop() {
  uint8_t minutes = 9;
  uint8_t seconds = 59;

  while (minutes > 0 && seconds > 0) {
    uint8_t data[] = {
      display.encodeDigit(minutes / 10),
      display.encodeDigit(minutes % 10),  
      display.encodeDigit(seconds / 10),
      display.encodeDigit(seconds % 10)
    };

    data[1] |= 0x80U; // Turn on colon by setting bit 7

    display.setSegments(data, 4U, 0U);

    if (seconds > 0) {
      seconds--;
    } else {
      minutes--;
      seconds = 0;
    }

    delay(1000);
  }
}
