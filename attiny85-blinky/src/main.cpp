#include <Arduino.h>

// Very fancy version :)

// PB5 PCINT5 | ADC0 | RESET
// PB4 PCINT4 | ADC2 | USB- | PWM4
// PB3 PCINT3 | ADC3 | USB+
// PB2 PCINT2 | ADC1 | SCLK | SCL | INT0
// PB1 PCINT1 | PWM1 | MISO | LED
// PB0 PCINT0 | PWM0 | MOSI | SDA

#define PIN_LED PIN1      // some versions have the led on PIN0
#define BLINK_PERIOD 500  // ms between state changes

//

static uint8_t ledState = LOW;

void initLed() {
  pinMode(PIN_LED, OUTPUT);
}

void updateLedState() {
  digitalWrite(PIN_LED, ledState);
}

void toggleLedState() {
  ledState = ledState == LOW ? HIGH : LOW;
}

//

void setup() {
  initLed();
}

void loop() {
  updateLedState();
  toggleLedState();

  delay(BLINK_PERIOD);
}
