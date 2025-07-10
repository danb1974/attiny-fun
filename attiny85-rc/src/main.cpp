#include <Arduino.h>

#define PIN_LED PIN0
#define PIN_RC PIN2

volatile uint32_t lastPulseStart = 0;
volatile uint32_t lastPulseWidth = 0;

void rcInterrupt() {
    uint8_t pinValue = digitalRead(PIN_RC);
    uint32_t now = micros();

    if (pinValue == 1) {
        // raising
        lastPulseStart = now;
        return;
    }

    // falling
    if (lastPulseStart == 0) {
        return;
    }

    uint32_t pulseWidth = now - lastPulseStart;
    if (pulseWidth < 1000 || pulseWidth > 2000) {
        // invalid
        pulseWidth = 0;
    }

    lastPulseWidth = pulseWidth;
}

void setup() {
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, 0);

    pinMode(PIN_RC, INPUT);
    attachInterrupt(0, rcInterrupt, CHANGE);
}

void loop() {}
