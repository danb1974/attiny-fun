#define NO_CLOCK_CORRECTION 1
#include <FastLED.h>

#define LED_PIN     0U
#define LED_COUNT   16U

static CRGB ledStrip[LED_COUNT];

void rainbow(uint8_t hueOffset) {
  for(uint8_t ledIdx = 0; ledIdx < LED_COUNT; ledIdx++) {
    uint8_t hue = static_cast<uint8_t>(hueOffset + ledIdx * 10);
    ledStrip[ledIdx] = CHSV(hue, 255, 255);
  }

  FastLED.show();
}

void digitalRGB(uint8_t rgbBits) {
    uint8_t r = rgbBits & 0x01 ? 255 : 0;
    uint8_t g = rgbBits & 0x02 ? 255 : 0;
    uint8_t b = rgbBits & 0x04 ? 255 : 0;

    for (uint8_t ledIdx = 0; ledIdx < LED_COUNT; ledIdx++) {
      ledStrip[ledIdx] = CRGB(r, g, b);
    }

    FastLED.show();
}

void setup() {
  // declare leds
  FastLED.addLeds<WS2812, LED_PIN, GRB>(ledStrip, LED_COUNT);

  // configure leds
  FastLED.setBrightness(10);
}

void loop() {
  for (uint8_t i = 0; i <= 255; i++) {
    rainbow(i);
    delay(10);
  }
}
