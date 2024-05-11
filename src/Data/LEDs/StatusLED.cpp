
#include "StatusLED.h"

void StatusLED::updateLEDs() {
    FlightStatus status = flight->getStatus();
    switch (status) {
        case IDLE:
            fill_solid(leds, NUM_LEDS, CRGB::Blue);
            break;
        case LAUNCH_PREP:
            fill_solid(leds, NUM_LEDS, CRGB::Yellow);
            break;
        case LAUNCHED:
            fill_solid(leds, NUM_LEDS, CRGB::Green);
            break;
        case IN_FLIGHT:
            fill_solid(leds, NUM_LEDS, CRGB::Red);
            break;
        case LANDED:
            fill_solid(leds, NUM_LEDS, CRGB::Purple);
            break;
    }
    FastLED.show();
}