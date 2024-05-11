#ifndef ROCKET_H
#define ROCKET_H

#include <vector>
#include "Flight/Flight.h"
#include "Data/LEDs/StatusLED.h"
#include "Data/Timer.h"
#include "Data/DataCard/DataCard.h"
#include "Sensors/DeviceRoutine.h"
#include "Sensors/LSM/LSMSensor.h"
#include "Sensors/TouchScreenTFT/TouchScreenTFT.h"
#include "Sensors/BluefriutLE/BluefruitLE.h"
#include "Sensors/TeensyBMP/Teensy_BMP180.h"
class Rocket {
public:
    Rocket();
    void setup();
    void loop();

private:
    StatusLED statusLED;
    Flight flight;
    Timer timer;
    DataCard dataCard;
    TouchScreenTFT touchScreen;
    LSMSensor adaFruitLSM;
    BluefruitLE ble;
    Teensy_BMP180 bmp180;
    bool solenoidState;
    int DEBUG_COUNTER;
    std::vector<DeviceRoutine*> devices;

    const bool WRITE_TO_SD = true;
    static const int SOLENOID_U7 = 29;
    static const int SOLENOID_U8 = 35;
    static const int DEBUG_LOG = true;
    static const int SPICLOCK = 600000;
    static const int REMOTE_DETONATION_SAFE_STATE = LOW;
    static const int REMOTE_DETONATION_ARMED_STATE = HIGH;

    void initDevices();
    void debug();
    void toggleSolenoid();
    void useTouchScreen();
    public:

};

#endif