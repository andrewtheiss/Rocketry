#ifndef _FLIGHT_H
#define _FLIGHT_H

#include "../Sensors/DeviceRoutine.h"
#include "FlightStatus.h"
#include "../Data/LEDs/StatusLED.h"
#include "../Sensors/LSM/LSMSensor.h"
#include "../Sensors/TeensyBMP/Teensy_BMP180.h"
#include "../Data/Timer.h"
#include <vector>

class StatusLED;

class Flight : public DeviceRoutine {
private:
    FlightStatus currentStatus;
    StatusLED* m_pStatusLED;
    std::vector<DeviceRoutine*> m_devices;
    LSMSensor* m_lsmSensor;
    Teensy_BMP180* m_bmpSensor;
    Timer* m_pTimer;

    
    // Flight logic variables
    static const int SAMPLE_SIZE = 10;
    static const float LAUNCH_G_THRESHOLD = 0.2;
    static const float LAUNCH_HEIGHT_THRESHOLD = 2.0f;

    float elevationSamples[SAMPLE_SIZE];
    float accelerationSamples[SAMPLE_SIZE];
    int sampleIndex;
    float averageElevation;
    float averageAcceleration;
    unsigned long lastSampleTime;
    unsigned long launchDetectionStartTime;

    // Launch checks
    bool highGDetected = false;
    bool elevationGainDetected = false;

    
    // Before flight
    void getSensorIdleAverages();
    void LoopDataRecording();
    void LoopIdleLaunchReady();

    // In-flight methods (implemented in Flight_Logic.cpp)
    void LoopAscent();
    void LoopAfterApogee();
    void LoopFlightComplete();

public:
    Flight();

    void init(StatusLED* statusLED, const std::vector<DeviceRoutine*>& devices, Timer* timer);
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