#include "Data/Timer.h"
#include "Data/DataCard/DataCard.h"
#include "Sensors/DeviceRoutine.h"
#include "Sensors/LSM/LSMSensor.h"
#include "Sensors/TouchScreenTFT/TouchScreenTFT.h"
#include "Sensors/BluefriutLE/BluefruitLE.h"
#include "Sensors/TeensyBMP/Teensy_BMP180.h"

#define SOLENOID_U7 29
#define SOLENOID_U8 35
#define DEBUG_LOG true
#define SPICLOCK 600000

#ifdef DEBUG_LOG
  int DEBUG_COUNTER = 0;
  
#endif


Timer timer;
DataCard dataCard;

// Device Routines
TouchScreenTFT touchScreen;
LSMSensor adaFruitLSM;
BluefruitLE ble;
Teensy_BMP180 bmp180(&Wire);

// Device routine settings
SPISettings settingsTouchScreen(SPICLOCK, MSBFIRST, SPI_MODE0); // Adjust as necessary for touch screen
SPISettings settingsLSMSensor(SPICLOCK, MSBFIRST, SPI_MODE0); // Adjust for LSM sensor
SPISettings settingsBluefruit(SPICLOCK, MSBFIRST, SPI_MODE0); // Adjust for Bluefruit LE module
const bool WRITE_TO_SD = true;

DeviceRoutine* devices[] = { 
  //&touchScreen,     // Touchscreen
  &adaFruitLSM,     // LSM 9-axis gyro/accel/mag
  &ble,             // Bluefruit LE
  &bmp180           // Barometric pressure sensor
};
const int numDevices = sizeof(devices) / sizeof(devices[0]);

#define REMOTE_DETONATION_SAFE_STATE LOW
#define REMOTE_DETONATION_ARMED_STATE HIGH
bool solenoidState = REMOTE_DETONATION_SAFE_STATE;

void setupDataCard() {
    const char dataToWrite[] = "Hello, SD card!";
    if (dataCard.writeData(dataToWrite)) {
        Serial.println("Write successful");
    } else {
        Serial.println("Write failed");
    }
}

void setPinState() {
  digitalWrite(SOLENOID_U7, REMOTE_DETONATION_SAFE_STATE);
  digitalWrite(SOLENOID_U8, REMOTE_DETONATION_SAFE_STATE);
  pinMode(LSM9DS0_CSG, OUTPUT); // Gyro CS pin
  pinMode(LSM9DS0_CSXM, OUTPUT); // Accel/Mag CS pin
  pinMode(TOUCH_CS, OUTPUT); // Touchscreen CS pin
  pinMode(TFT_CS, OUTPUT); // Touchscreen CS pin
  pinMode(BLUEFRUIT_SPI_CS, OUTPUT); // Bluefruit CS pin
  delay(100);
}

void setup()
{
  // Setup SD Card
  dataCard.init();
  setupDataCard();

  // Setup devices
  timer.start();
  for (int i = 0; i < numDevices; i++) {
      devices[i]->init();
      delay(10);
  }
  
  setPinState();
}

void toggleSolenoid()
{
  Serial.println("Toggling solenoid");
  digitalWrite(SOLENOID_U7, REMOTE_DETONATION_ARMED_STATE);
  digitalWrite(SOLENOID_U8, REMOTE_DETONATION_ARMED_STATE);
  delay(4000);
  digitalWrite(SOLENOID_U7, REMOTE_DETONATION_SAFE_STATE);
  digitalWrite(SOLENOID_U8, REMOTE_DETONATION_SAFE_STATE);
  delay(3000);
  Serial.println("Toggling solenoid back off");
}

void useTouchScreen() {
    // Ensure both LSM9DS0 CS lines are inactive
    // digitalWrite(LSM9DS0_CSG, HIGH);
    // digitalWrite(LSM9DS0_CSXM, HIGH);
    //pinMode(TFT_CS, HIGH); // Touchscreen CS pin
    // digitalWrite(TOUCH_CS, LOW); // Activate touchscreen
    SPI.beginTransaction(settingsTouchScreen);
    // Perform touchscreen operations
    // ================================================

    unsigned int touchCommand = touchScreen.checkTouch();
    switch (touchCommand)
      {
        case 1:
          toggleSolenoid();
          break;
        default:
          break;
    }    

    // ================================================
    // digitalWrite(TOUCH_CS, HIGH); // Deactivate touchscreen
    SPI.endTransaction();
    //pinMode(TFT_CS, LOW); // Touchscreen CS pin
    
}

void useLSM9DS0Gyro() {
    // Ensure touchscreen and LSM9DS0 Accel/Mag CS lines are inactive
    // digitalWrite(TOUCH_CS, HIGH);
    // digitalWrite(LSM9DS0_CSXM, HIGH);

    SPI.beginTransaction(settingsLSMSensor);
    // digitalWrite(LSM9DS0_CSG, LOW); // Activate LSM9DS0 Gyro
    // Perform LSM9DS0 Gyro operations
    adaFruitLSM.printGyro();
    // digitalWrite(LSM9DS0_CSG, HIGH); // Deactivate LSM9DS0 Gyro
    SPI.endTransaction();
}

void useLSM9DS0AccelMag() {
    // Ensure touchscreen and LSM9DS0 Gyro CS lines are inactive
    // digitalWrite(TOUCH_CS, HIGH);
    // digitalWrite(LSM9DS0_CSG, HIGH);

    SPI.beginTransaction(settingsLSMSensor);
    // digitalWrite(LSM9DS0_CSXM, LOW); // Activate LSM9DS0 Accel/Mag
    // Perform LSM9DS0 Accel/Mag operations
    adaFruitLSM.refreshForRead();
    
    if (WRITE_TO_SD) {
        const char* data = adaFruitLSM.getFormattedAcceleration();
        if (!dataCard.writeData(data)) {
            Serial.println("Failed to write data to SD card.");
        }
    }

    adaFruitLSM.refreshForRead();
    adaFruitLSM.printAccel();
    // digitalWrite(LSM9DS0_CSXM, HIGH); // Deactivate LSM9DS0 Accel/Mag
    SPI.endTransaction();
}

void loop()
{
    if (ble.getCurrentState() == BluefruitLE::FIRE) {
      toggleSolenoid();
      ble.setPrepareToArm();
    }
    // Example of using the touchscreen
    //useTouchScreen();
    // Example of using the LSM9DS0 Gyro
    //useLSM9DS0Gyro();

    // Loop through each device
    for (DeviceRoutine* device : devices) {
        device->loop();
    }
    
    // Example of using the LSM9DS0 Accel/Mag
    //useLSM9DS0AccelMag();

  // sleep 0.1ms
  delay(10);

  #ifdef DEBUG_LOG
    if(DEBUG_COUNTER++ % 100 == 0) {
      Serial.println("Debugging");
      //toggleSolenoid();
      //delay(1000);
      timer.printElapsedTime();
      timer.printCurrentTime();
      double temperature, pressure, altitude;

      // Start and retrieve temperature
      if (bmp180.startTemperature()) {
        if (bmp180.getTemperature(temperature)) {
          Serial.print("Temperature: ");
          Serial.print(temperature);
          Serial.println(" C");
        } else {
          Serial.println("Error reading temperature");
        }
      } else {
        Serial.println("Error starting temperature measurement");
      }

      // Start and retrieve pressure
      if (bmp180.startPressure()) {
        if (bmp180.getPressure(pressure, temperature)) {
          Serial.print("Pressure: ");
          Serial.print(pressure);
          Serial.println(" mb");

          // Calculate altitude
          altitude = bmp180.altitude(pressure, 1013.25); // Assuming sea-level standard pressure is 1013.25 mb
          Serial.print("Altitude: ");
          Serial.print(altitude);
          Serial.println(" meters");
        } else {
          Serial.println("Error reading pressure");
        }
      } else {
        Serial.println("Error starting pressure measurement");
      }
    }   
   #endif
}

