#include <Arduino.h>

#define PIN_INT_LED PIN1
#define PIN_RC      PIN2

static volatile uint32_t lastPulseStartUs = 0U;
static volatile uint32_t lastPulseWidthUs = 0U;

void rcInterrupt() {
    uint32_t now = micros();

    uint8_t pinValue = digitalRead(PIN_RC);

    if (pinValue == 1) {
        // raising
        lastPulseStartUs = now;
        return;
    }

    // falling
    if (lastPulseStartUs == 0U) {
        return;
    }

    uint32_t pulseWidth = now - lastPulseStartUs;
    if (pulseWidth < 1000U || pulseWidth > 2000U) {
        // invalid
        pulseWidth = 0U;
    }

    lastPulseWidthUs = pulseWidth;
}

void setup() {
    pinMode(PIN_INT_LED, OUTPUT);
    digitalWrite(PIN_INT_LED, 0);

    pinMode(PIN_RC, INPUT);
    attachInterrupt(0, rcInterrupt, CHANGE);
}

void loop() {
    int8_t switchPos = 0;
    static uint8_t cycle = 0;

    cycle++;

    if (lastPulseWidthUs >= 1000U && lastPulseWidthUs <= 2000U) {
        if (lastPulseWidthUs < 1250U) {
            switchPos = -1;
        }
        if (lastPulseWidthUs > 1750U) {
            switchPos = 1;
        }

        if (switchPos == -1) {
            digitalWrite(PIN_INT_LED, cycle & 0x01);
        } else if (switchPos == 1) {
            digitalWrite(PIN_INT_LED, 1);
        } else {
            digitalWrite(PIN_INT_LED, 0);
        }
    }

    delay(500);
}
