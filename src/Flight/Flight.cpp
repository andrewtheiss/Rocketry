#include "Flight.h"
#include "../Data/LEDs/StatusLED.h"

void Flight::setStatus(FlightStatus status) {
    currentStatus = status;
    Serial.print("Updateing status to: " + status);
    if (m_pStatusLED) {
        m_pStatusLED->updateLEDs(); // Update LEDs whenever the status changes
    }
}

void Flight::init(StatusLED* statusLED) {
    m_pStatusLED = statusLED;
    setStatus(IDLE);
}

void Flight::updateFlight() {
    // If accel is greater than x amount for a certain time, change status to in flight
    // Placeholder for future flight logic
}

void Flight::updateStatusIfOK(FlightStatus status) {
    if (currentStatus < ERROR) {
        setStatus(status);
    }
}