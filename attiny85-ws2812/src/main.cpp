#define NO_CLOCK_CORRECTION 1
#include <FastLED.h>

#define LED_PIN     0
#define NUM_LEDS    16

CRGB leds[NUM_LEDS];

void rainbow() {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(i * 10, 255, 255);
  }
  FastLED.show();
}

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(10);

  rainbow();
  delay(1000);
}

void loop() {
  for (uint8_t i = 1; i <= 7; i++) {
    for (uint8_t led = 0; led < NUM_LEDS; led++) {
      uint8_t r = i & 0x01 ? 255 : 0;
      uint8_t g = i & 0x02 ? 255 : 0;
      uint8_t b = i & 0x04 ? 255 : 0;
      leds[led] = CRGB(r, g, b);
    }
    FastLED.show();

    delay(1000);
  }
}
