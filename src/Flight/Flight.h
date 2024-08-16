#ifndef _FLIGHT_H
#define _FLIGHT_H

#include "../Sensors/DeviceRoutine.h"
#include "FlightStatus.h"
#include "../Data/LEDs/StatusLED.h"

class StatusLED;

class Flight : public DeviceRoutine {
private:
    FlightStatus currentStatus;
    StatusLED* m_pStatusLED;
public:
    Flight() : currentStatus(IDLE), m_pStatusLED(nullptr) {
    }

    void init(StatusLED* statusLED);

    void setStatus(FlightStatus status);

    void updateStatusIfOK(FlightStatus status);

    FlightStatus getStatus() const {
        return currentStatus;
    }
    
    // DeviceRoutine virtual methods
    virtual void init() override;
    virtual void loop() override;
    virtual const char* getName() override;


    virtual void getData(char data[]) override {
        // Implement or leave blank for now
    }

};

#endif