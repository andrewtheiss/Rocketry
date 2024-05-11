#ifndef STATUSLED_H
#define STATUSLED_H

#include <FastLED.h>
#include "../../Flight/Flight.h"

class Flight;

// How many LEDs in your strip?
#define NUM_LEDS 10
#define DATA_PIN 35


class StatusLED {
private:
    Flight* flight;
    CRGB leds[NUM_LEDS];

public:
    StatusLED(Flight* flightPtr) : flight(flightPtr) {
        FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    }

    void updateLEDs();
};

#endif