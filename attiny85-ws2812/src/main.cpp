#define NO_CLOCK_CORRECTION 1
#include <FastLED.h>

#define LED_PIN     0U
#define LED_COUNT   16U

static CRGB ledStrip[LED_COUNT];

void rainbowFlow(uint8_t hueOffset) {
  for (uint8_t ledIdx = 0; ledIdx < LED_COUNT; ledIdx++) {
    uint8_t hue = static_cast<uint8_t>(hueOffset + ledIdx * 10);
    ledStrip[ledIdx] = CHSV(hue, 255, 255);
  }

  FastLED.show();
}

void rainbowLines(uint8_t hueOffset) {
  for (uint8_t ledIdx = 0; ledIdx < LED_COUNT; ledIdx++) {
    uint8_t hue = static_cast<uint8_t>(hueOffset + (ledIdx / 4) * 20);
    ledStrip[ledIdx] = CHSV(hue, 255, 255);
  }

  FastLED.show();
}

void rainbowDot(uint8_t hueOffset) {
  for (uint8_t ledIdx = 0; ledIdx < LED_COUNT; ledIdx++) {
    ledStrip[ledIdx] = CHSV(0, 0, 0);
  }

  uint8_t ledIdx = hueOffset % LED_COUNT;
  uint8_t hue = static_cast<uint8_t>(hueOffset + (ledIdx / 4) * 20);
  ledStrip[ledIdx] = CHSV(hue, 255, 255);

  FastLED.show();
}

void policeLights(bool red, bool blue) {
  for (uint8_t ledIdx = 0; ledIdx < LED_COUNT; ledIdx++) {
    ledStrip[ledIdx] = CHSV(0, 0, 0);
  }

  if (red) {
    for (uint8_t ledIdx = 0; ledIdx < LED_COUNT / 2; ledIdx++) {
     ledStrip[ledIdx] = CRGB(255, 0, 0);
    }
  }

  if (blue) {
    for (uint8_t ledIdx = LED_COUNT / 2; ledIdx < LED_COUNT; ledIdx++) {
     ledStrip[ledIdx] = CRGB(0, 0, 255);
    }
  }

  FastLED.show();
}

void setup() {
  // declare leds
  FastLED.addLeds<WS2812, LED_PIN, GRB>(ledStrip, LED_COUNT);

  // configure leds
  FastLED.setBrightness(10);
}

#define MODEL 1

void loop() {
  switch (MODEL) {
    case 1:
      for (uint8_t c = 0; c < 2; c++) {
        for (uint8_t i = 0; i < 3; i++) {
          policeLights(c == 0, c == 1);
          delay(100);
          policeLights(false, false);
          delay(100);
        }
        delay(200);
      }
      break;

    case 2:
      for (uint8_t i = 0; i <= 255; i++) {
        rainbowFlow(i);
        delay(10);
      }
      break;

    case 3:
      for (uint8_t i = 0; i <= 255; i++) {
        rainbowLines(i);
        delay(10);
      }
      break;

    case 4:
      for (uint8_t i = 0; i <= 255; i++) {
        rainbowDot(i);
        delay(100);
      }
      break;
  }
}
