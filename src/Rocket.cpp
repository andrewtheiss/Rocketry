#include "Rocket.h"

Rocket::Rocket() 
    : bmp180(&Wire), 
      solenoidState(REMOTE_DETONATION_SAFE_STATE), 
      DEBUG_COUNTER(0) {}

void Rocket::setup() {
    Serial.begin(9600);
    dataCard.init();
    timer.start();
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
    DeviceRoutine* devices[] = { 
        &adaFruitLSM,     // LSM 9-axis gyro/accel/mag
        &ble,             // Bluefruit LE
        &bmp180           // Barometric pressure sensor
    };
    const int numDevices = sizeof(devices) / sizeof(devices[0]);
    for (int i = 0; i < numDevices; i++) {
        devices[i]->init();
        delay(10);
    }
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

void Rocket::useLSM9DS0Gyro() {
    SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
    adaFruitLSM.printGyro();
    SPI.endTransaction();
}

void Rocket::useLSM9DS0AccelMag() {
    SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
    adaFruitLSM.refreshForRead();
    if (WRITE_TO_SD) {
        const char* data = adaFruitLSM.getFormattedAcceleration();
        if (!dataCard.writeData(data)) {
            Serial.println("Failed to write data to SD card.");
        }
    }
    adaFruitLSM.refreshForRead();
    adaFruitLSM.printAccel();
    SPI.endTransaction();
}

void Rocket::writeDataToSD() {
    const char dataToWrite[] = "Hello, SD card!";
    if (dataCard.writeData(dataToWrite)) {
        Serial.println("Write successful");
    } else {
        Serial.println("Write failed");
    }
}

void Rocket::debug() {
    if (DEBUG_LOG && DEBUG_COUNTER++ % 100 == 0) {
        Serial.println("Debugging");
        timer.printElapsedTime();
        timer.printCurrentTime();
        double temperature, pressure, altitude;
        if (bmp180.startTemperature() && bmp180.getTemperature(temperature)) {
            Serial.print("Temperature: ");
            Serial.print(temperature);
            Serial.println(" C");
        } else {
            Serial.println("Error reading temperature");
        }
        if (bmp180.startPressure() && bmp180.getPressure(pressure, temperature)) {
            Serial.print("Pressure: ");
            Serial.print(pressure);
            Serial.println(" mb");
            altitude = bmp180.altitude(pressure, 1013.25);
            Serial.print("Altitude: ");
            Serial.print(altitude);
            Serial.println(" meters");
        } else {
            Serial.println("Error reading pressure");
        }
    }
}

void Rocket::loop() {
    if (ble.getCurrentState() == BluefruitLE::FIRE) {
        toggleSolenoid();
        ble.setPrepareToArm();
    }
    DeviceRoutine* devices[] = { 
        &adaFruitLSM,     // LSM 9-axis gyro/accel/mag
        &ble,             // Bluefruit LE
        &bmp180           // Barometric pressure sensor
    };
    const int numDevices = sizeof(devices) / sizeof(devices[0]);
    for (int i = 0; i < numDevices; i++) {
        devices[i]->loop();
    }
    debug();
    delay(10);
}
