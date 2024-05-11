#include "Rocket.h"

Rocket::Rocket() 
    : bmp180(&Wire), 
      solenoidState(REMOTE_DETONATION_SAFE_STATE), 
      DEBUG_COUNTER(0) {
        devices = { 
            &adaFruitLSM,     // LSM 9-axis gyro/accel/mag
            //&touchScreen,   // Touchscreen (commented out)
            &ble,             // Bluefruit LE
            &bmp180           // Barometric pressure sensor
        };
      }

void Rocket::setup() {
    Serial.begin(9600);
    timer.start();
    dataCard.init(timer);
    initDevices();
    digitalWrite(SOLENOID_U7, REMOTE_DETONATION_SAFE_STATE);
    digitalWrite(SOLENOID_U8, REMOTE_DETONATION_SAFE_STATE);
    pinMode(LSM9DS0_CSG, OUTPUT); // Gyro CS pin
    pinMode(LSM9DS0_CSXM, OUTPUT); // Accel/Mag CS pin
    pinMode(TOUCH_CS, OUTPUT); // Touchscreen CS pin
    pinMode(TFT_CS, OUTPUT); // Touchscreen CS pin
    pinMode(BLUEFRUIT_SPI_CS, OUTPUT); // Bluefruit CS pin
    delay(100);
}

void Rocket::initDevices() {
    for (DeviceRoutine* device : devices) {
        device->init();
        //device->setLoggingEnabled(true); // Enable logging for all devices
        delay(10);
    }
    dataCard.initFiles(devices);
}

void Rocket::toggleSolenoid() {
    Serial.println("Toggling solenoid");
    digitalWrite(SOLENOID_U7, REMOTE_DETONATION_ARMED_STATE);
    digitalWrite(SOLENOID_U8, REMOTE_DETONATION_ARMED_STATE);
    delay(4000);
    digitalWrite(SOLENOID_U7, REMOTE_DETONATION_SAFE_STATE);
    digitalWrite(SOLENOID_U8, REMOTE_DETONATION_SAFE_STATE);
    delay(3000);
    Serial.println("Toggling solenoid back off");
}

void Rocket::useTouchScreen() {
    SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
    unsigned int touchCommand = touchScreen.checkTouch();
    switch (touchCommand) {
        case 1:
            toggleSolenoid();
            break;
        default:
            break;
    }
    SPI.endTransaction();
}

void Rocket::debug() {
    if (DEBUG_LOG && DEBUG_COUNTER++ % 100 == 0) {
        Serial.println("Debugging");
        timer.printElapsedTime();
        timer.printCurrentTime();
    }
}

void Rocket::loop() {
    if (ble.getCurrentState() == BluefruitLE::FIRE) {
        toggleSolenoid();
        ble.setPrepareToArm();
    }

    for (DeviceRoutine* device : devices) {
        device->loop();
        if (device->isLoggingEnabled()) {
            if (!dataCard.writeData(device)) {
                Serial.printf("Failed to write data for device: %s\n", device->getName());
            }
        }
    }
    debug();
    delay(10);
}
