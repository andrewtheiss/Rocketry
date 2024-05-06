#include "Data/Timer.h"
#include "Data/DataCard/DataCard.h"
#include "Sensors/DeviceRoutine.h"
#include "Sensors/LSM/LSMSensor.h"
#include "Sensors/TouchScreenTFT/TouchScreenTFT.h"
#include "Sensors/BluefriutLE/BluefruitLE.h"

#define SOLENOID_U7 29
#define SOLENOID_U8 35
#define DEBUG_LOG true
#define SPICLOCK 40000000

#ifdef DEBUG_LOG
  int DEBUG_COUNTER = 0;
  
#endif


Timer timer;
DataCard dataCard;

// Device Routines
TouchScreenTFT touchScreen;
LSMSensor adaFruitLSM;
BluefruitLE ble;

// Device routine settings
SPISettings settingsTouchScreen(SPICLOCK, MSBFIRST, SPI_MODE0); // Adjust as necessary for touch screen
SPISettings settingsLSMSensor(SPICLOCK, MSBFIRST, SPI_MODE0); // Adjust for LSM sensor
SPISettings settingsBluefruit(SPICLOCK, MSBFIRST, SPI_MODE0); // Adjust for Bluefruit LE module
const bool WRITE_TO_SD = true;

DeviceRoutine* devices[] = { &touchScreen,  &adaFruitLSM, &ble };
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

void setup()
{
  digitalWrite(SOLENOID_U7, REMOTE_DETONATION_SAFE_STATE);
  digitalWrite(SOLENOID_U8, REMOTE_DETONATION_SAFE_STATE);
  dataCard.init();
  setupDataCard();
  // Setup code here
  pinMode(LSM9DS0_CSG, OUTPUT); // Gyro CS pin
  pinMode(LSM9DS0_CSXM, OUTPUT); // Accel/Mag CS pin
  pinMode(TOUCH_CS, OUTPUT); // Touchscreen CS pin
  pinMode(TFT_CS, OUTPUT); // Touchscreen CS pin
  pinMode(BLUEFRUIT_SPI_CS, OUTPUT); // Bluefruit CS pin

  for (int i = 0; i < numDevices; i++) {
      devices[i]->init();
      delay(10);
  }
  pinMode(LSM9DS0_CSG, OUTPUT); // Gyro CS pin
  pinMode(LSM9DS0_CSXM, OUTPUT); // Accel/Mag CS pin
  pinMode(TOUCH_CS, OUTPUT); // Touchscreen CS pin
  pinMode(TFT_CS, OUTPUT); // Touchscreen CS pin
  pinMode(BLUEFRUIT_SPI_CS, OUTPUT); // Bluefruit CS pin
  delay(100);
  timer.start();
}

void toggleSolenoid()
{
  Serial.println("Toggling solenoid");
  digitalWrite(SOLENOID_U7, REMOTE_DETONATION_ARMED_STATE);
  delay(2000);
  digitalWrite(SOLENOID_U7, REMOTE_DETONATION_SAFE_STATE);

  delay(500);
  digitalWrite(SOLENOID_U8, REMOTE_DETONATION_ARMED_STATE);
  delay(2000);
  digitalWrite(SOLENOID_U8, REMOTE_DETONATION_SAFE_STATE);
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
    // Example of using the touchscreen
    //useTouchScreen();
    // Example of using the LSM9DS0 Gyro
    //useLSM9DS0Gyro();

    // Loop through each device
    for (DeviceRoutine* device : devices) {
        device->loop();
    }
    
    // Example of using the LSM9DS0 Accel/Mag
    useLSM9DS0AccelMag();

  // sleep 0.1ms
  delay(10);

  #ifdef DEBUG_LOG
    if(DEBUG_COUNTER++ % 100 == 0) {
      Serial.println("Debugging");
      toggleSolenoid();
      delay(1000);
      timer.printElapsedTime();
      timer.printCurrentTime();
    }
    
   #endif

  // // Bluefruit LE Module Operation
  // SPI.beginTransaction(settingsBluefruit);
  // // digitalWrite(BLUEFRUIT_SPI_CS, LOW); // Assuming BLUEFRUIT_CS is the CS pin for your Bluefruit module
  // // Perform Bluefruit operations
  // // digitalWrite(BLUEFRUIT_SPI_CS, HIGH);
  // SPI.endTransaction(); 


}

