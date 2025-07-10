#include <Arduino.h>
#include <TM1637Display.h>

#define CLK 2
#define DIO 0
#define LED 1

TM1637Display display(CLK, DIO);

void clearDisplay() {
  uint8_t data[] = {0, 0, 0, 0};
  display.setSegments(data, 4U, 0U);
}

void segmentTest() {
  uint8_t data[] = {1, 1, 1, 1};

  for (uint8_t i = 0; i < 6; i++) {
    for (uint8_t digit = 0; digit < 4; digit++) {
      data[digit] <<= 1;
    }
    display.setSegments(data, 4U, 0U);

    delay(300);
  }

  clearDisplay();
  delay(300);
}

void setup() {
  display.setBrightness(0x03); // 0x0f is maximum

  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);

  segmentTest();
}

void loop() {
  static uint8_t minutes = 9;
  static uint8_t seconds = 59;

  if (minutes > 0 && seconds > 0) {
    uint8_t data[] = {
      display.encodeDigit(minutes / 10),
      display.encodeDigit(minutes % 10),  
      display.encodeDigit(seconds / 10),
      display.encodeDigit(seconds % 10)
    };

    // Turn on colon by setting bit 7
    // We toggle it every second
    data[1] |= (seconds & 0x01) << 7;

    display.setSegments(data, 4U, 0U);

    if (seconds > 0) {
      seconds--;
    } else {
      minutes--;
      seconds = 59;
    }
  } else {
    clearDisplay();
  }

  delay(1000);
}
