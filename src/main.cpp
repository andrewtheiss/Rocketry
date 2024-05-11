#include "Rocket.h"
// #include <FastLED.h>
 
// // How many LEDs in your strip?
// #define NUM_LEDS 10

// #define DATA_PIN 17

// // Define the array of LEDs
// CRGB leds[NUM_LEDS];

Rocket rocket;

void setup() {
    rocket.setup();
    // FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
    rocket.loop();
    //  leds[0] = CRGB::Red;
    // FastLED.show();
    // delay(500);
    // // Now turn the LED off, then pause
    // leds[0] = CRGB::Black;
    // FastLED.show();
    // delay(500);
}