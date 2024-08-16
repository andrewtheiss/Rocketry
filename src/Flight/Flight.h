#ifndef _FLIGHT_H
#define _FLIGHT_H

#include "../Sensors/DeviceRoutine.h"
#include "FlightStatus.h"
#include "../Data/LEDs/StatusLED.h"
#include "../Sensors/LSM/LSMSensor.h"
#include "../Sensors/TeensyBMP/Teensy_BMP180.h"
#include <vector>

class StatusLED;

class Flight : public DeviceRoutine {
private:
    FlightStatus currentStatus;
    StatusLED* m_pStatusLED;
    std::vector<DeviceRoutine*> m_devices;
    LSMSensor* m_lsmSensor;
    Teensy_BMP180* m_bmpSensor;

public:
    Flight() : currentStatus(IDLE), m_pStatusLED(nullptr), m_lsmSensor(nullptr), m_bmpSensor(nullptr) {}

    void init(StatusLED* statusLED, const std::vector<DeviceRoutine*>& devices);
    void setStatus(FlightStatus status);
    void updateStatusIfOK(FlightStatus status);
    FlightStatus getStatus() const { return currentStatus; }

    void setLSMSensor(LSMSensor* sensor) { m_lsmSensor = sensor; }
    void setBMPSensor(Teensy_BMP180* sensor) { m_bmpSensor = sensor; }

    // DeviceRoutine virtual methods
    virtual void init() override;
    virtual void loop() override;
    virtual void getData(char data[]) override;
    virtual const char* getName() override;
};

#endif // _FLIGHT_H