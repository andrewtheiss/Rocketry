#include "Rocket.h"

Rocket::Rocket() 
    : bmp180(&Wire), 
      solenoidState(REMOTE_DETONATION_SAFE_STATE), 
      DEBUG_COUNTER(0),
      statusLED(&flight)  {
        devices = { 
            &adaFruitLSM,     // LSM 9-axis gyro/accel/mag
            //&touchScreen,   // Touchscreen (commented out)
            &ble,             // Bluefruit LE
            &bmp180           // Barometric pressure sensor
        };
        flight.init(&statusLED);
      }

void Rocket::setup() {
    delay(100);
    Serial.print("Initializing rocket");
    Serial.begin(9600);
    timer.start();
    
    // Check to make sure data is being written
    if (!dataCard.init(timer)) {
        flight.setStatus(ERROR_NO_SD);
    };

    initDevices();
    initFlight();
    digitalWrite(SOLENOID_U7, REMOTE_DETONATION_SAFE_STATE);
    digitalWrite(SOLENOID_U8, REMOTE_DETONATION_SAFE_STATE);
    pinMode(LSM9DS0_CSG, OUTPUT); // Gyro CS pin
    pinMode(LSM9DS0_CSXM, OUTPUT); // Accel/Mag CS pin
    pinMode(TOUCH_CS, OUTPUT); // Touchscreen CS pin
    pinMode(TFT_CS, OUTPUT); // Touchscreen CS pin
    pinMode(BLUEFRUIT_SPI_CS, OUTPUT); // Bluefruit CS pin
    
    delay(100);
    flight.updateStatusIfOK(DATA_RECORDING);
}

void Rocket::initFlight() {
    dataCard.initFile(&flight);
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

    // Update the flight
    flight.loop();

    debug();
    delay(10);
}
