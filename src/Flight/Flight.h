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
    const float LAUNCH_G_THRESHOLD = 0.5f;  // 1 g
    const float LAUNCH_HEIGHT_THRESHOLD = 200.0f;  // 200 meters unit we launch

    float elevationSamples[SAMPLE_SIZE];
    float accelerationSamples[SAMPLE_SIZE];
    int sampleIndex;
    float averageElevation;
    float averageAcceleration;
    unsigned long lastSampleTime;
    unsigned long launchDetectionStartTime;

    // Launch checks
    bool highGDetected = true;
    bool elevationGainDetected = true;

    // Flight apogee variables 
    uint32_t timerForApogeeCheckToBegin = 0;
    bool timerForApogeeFinished = false;
    
    // Apogee test variables
    const int size = 50;
    float values[50] = {0};
    float sum = 0.0f;
    float weight_sum = 0.0f;
    int current_index = 0;
    int count = 0;
    float maxElevation = -INFINITY;
    const float APOGEE_THRESHOLD = 5.0; // 5 meter decrease threshold
    const float G_FORCE_THRESHOLD = 0.1; // 0.1G threshold
    const int APOGEE_G_FORCE_COUNT = 15; // 15 consecutive low G-force readings for apogee
    int lowGForceCount = 0;


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