#include "StatusLED.h"

void StatusLED::updateLEDs() {
    FlightStatus status = flight->getStatus();
    switch (status) {
        case IDLE:
            fill_solid(leds, NUM_LEDS, CRGB::Yellow);
            break;
        case SENSOR_CALIBRATION_AND_SD_WRITE:
            fill_solid(leds, NUM_LEDS, CRGB::Green);
            break;
        case CALIBRATED_LAUNCH_READY:
            fill_solid(leds, NUM_LEDS, CRGB::White);
            break;
        case IN_FLIGHT_ASCENT:
            fill_solid(leds, NUM_LEDS, CRGB::Blue);
            break;
        case IN_FLIGHT_APOGEE:
            fill_solid(leds, NUM_LEDS, CRGB::Cyan);
            break;
        case IN_FLIGHT_MAIN_CHUTE_DEPLOYED:
            fill_solid(leds, NUM_LEDS, CRGB::Magenta);
            break;
        case LANDED:
            fill_solid(leds, NUM_LEDS, CRGB::Green);
            break;
        case ERROR:
        case ERROR_NO_SD:
            fill_solid(leds, NUM_LEDS, CRGB::Red);
            break;
        default:
            fill_solid(leds, NUM_LEDS, CRGB::White);
            break;
    }
    FastLED.show();
}