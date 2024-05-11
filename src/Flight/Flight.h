#ifndef _FLIGHT_H
#define _FLIGHT_H

#include "FlightStatus.h"
#include "../Data/LEDs/StatusLED.h"

class StatusLED;

class Flight {
private:
    FlightStatus currentStatus;
    StatusLED* m_pStatusLED;
public:
    Flight() : currentStatus(IDLE), m_pStatusLED(nullptr) {
    }

    void init(StatusLED* statusLED);

    void setStatus(FlightStatus status);

    FlightStatus getStatus() const {
        return currentStatus;
    }
};

#endif