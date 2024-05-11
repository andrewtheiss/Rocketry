#include "Flight.h"
#include "../Data/LEDs/StatusLED.h"

void Flight::setStatus(FlightStatus status) {
    currentStatus = status;
    if (m_pStatusLED) {
        m_pStatusLED->updateLEDs(); // Update LEDs whenever the status changes
    }
}

void Flight::init(StatusLED* statusLED) {
    m_pStatusLED = statusLED;
    setStatus(IDLE);
}