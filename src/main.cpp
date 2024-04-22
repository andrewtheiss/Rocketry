#include "Sensors/DeviceRoutine.h"
#include "Sensors/LSM/LSMSensor.h"
#include "Sensors/TouchScreenTFT/TouchScreenTFT.h"
#include "Sensors/BluefriutLE/BluefruitLE.h"

#define SOLENOID_PIN 21

TouchScreenTFT touchScreen;
LSMSensor adaFruitLSM;
BluefruitLE ble;
SPISettings settingsTouchScreen(1000000, MSBFIRST, SPI_MODE0); // Adjust as necessary for touch screen
SPISettings settingsLSMSensor(1000000, MSBFIRST, SPI_MODE3); // Adjust for LSM sensor
SPISettings settingsBluefruit(4000000, MSBFIRST, SPI_MODE0); // Adjust for Bluefruit LE module

DeviceRoutine* devices[] = { &touchScreen, &adaFruitLSM, &ble };
const int numDevices = sizeof(devices) / sizeof(devices[0]);

#define REMOTE_DETONATION_SAFE_STATE LOW
#define REMOTE_DETONATION_ARMED_STATE HIGH
bool solenoidState = REMOTE_DETONATION_SAFE_STATE;

void setup()
{
  // Setup code here
  pinMode(LSM9DS0_CSG, OUTPUT); // Gyro CS pin
  pinMode(LSM9DS0_CSXM, OUTPUT); // Accel/Mag CS pin
  pinMode(TOUCH_CS, OUTPUT); // Touchscreen CS pin
  pinMode(BLUEFRUIT_SPI_CS, OUTPUT); // Bluefruit CS pin

  for (int i = 0; i < numDevices; i++) {
      devices[i]->init();
      delay(10);
  }
  delay(100);
}

void toggleSolenoid()
{
  Serial.println("Toggling solenoid");
  digitalWrite(SOLENOID_PIN, REMOTE_DETONATION_ARMED_STATE);
  delay(5000);
  digitalWrite(SOLENOID_PIN, REMOTE_DETONATION_SAFE_STATE);
  Serial.println("Toggling solenoid back off");
}

void useTouchScreen() {
    // Ensure both LSM9DS0 CS lines are inactive
    digitalWrite(LSM9DS0_CSG, HIGH);
    digitalWrite(LSM9DS0_CSXM, HIGH);

    SPI.beginTransaction(settingsTouchScreen);
    digitalWrite(TOUCH_CS, LOW); // Activate touchscreen
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
    digitalWrite(TOUCH_CS, HIGH); // Deactivate touchscreen
    SPI.endTransaction();
}

void useLSM9DS0Gyro() {
    // Ensure touchscreen and LSM9DS0 Accel/Mag CS lines are inactive
    digitalWrite(TOUCH_CS, HIGH);
    digitalWrite(LSM9DS0_CSXM, HIGH);

    SPI.beginTransaction(settingsLSMSensor);
    digitalWrite(LSM9DS0_CSG, LOW); // Activate LSM9DS0 Gyro
    // Perform LSM9DS0 Gyro operations
    adaFruitLSM.printGyro();
    digitalWrite(LSM9DS0_CSG, HIGH); // Deactivate LSM9DS0 Gyro
    SPI.endTransaction();
}

void useLSM9DS0AccelMag() {
    // Ensure touchscreen and LSM9DS0 Gyro CS lines are inactive
    digitalWrite(TOUCH_CS, HIGH);
    digitalWrite(LSM9DS0_CSG, HIGH);

    SPI.beginTransaction(settingsLSMSensor);
    digitalWrite(LSM9DS0_CSXM, LOW); // Activate LSM9DS0 Accel/Mag
    // Perform LSM9DS0 Accel/Mag operations
    
    adaFruitLSM.refreshForRead();
    adaFruitLSM.printAccel();
    digitalWrite(LSM9DS0_CSXM, HIGH); // Deactivate LSM9DS0 Accel/Mag
    SPI.endTransaction();
}

void loop()
{
    // Example of using the touchscreen
    useTouchScreen();
    
    // Example of using the LSM9DS0 Gyro
    //useLSM9DS0Gyro();
    
    // Example of using the LSM9DS0 Accel/Mag
    useLSM9DS0AccelMag();

  // sleep 0.1ms
  delay(10);


  // // Bluefruit LE Module Operation
  // SPI.beginTransaction(settingsBluefruit);
  // digitalWrite(BLUEFRUIT_SPI_CS, LOW); // Assuming BLUEFRUIT_CS is the CS pin for your Bluefruit module
  // // Perform Bluefruit operations
  // digitalWrite(BLUEFRUIT_SPI_CS, HIGH);
  // SPI.endTransaction(); 


//   // LSM Sensor Operation
//   SPI.beginTransaction(settingsLSMSensor);
//   digitalWrite(LSM9DS0_CSG, LOW); // Assuming LSM_CS is the CS pin for your LSM sensor
//   digitalWrite(LSM9DS0_CSXM, LOW); // Assuming LSM_CS is the CS pin for your LSM sensor
//   // Perform LSM sensor operations
//   adaFruitLSM.printMenu();
//   // Then wait for any serial data to come in:
//   while (!Serial.available())
//     ;
//   // Once serial data is received, call parseMenu to act on it:
//   adaFruitLSM.parseMenu(Serial.read());
//   digitalWrite(LSM9DS0_CSG, HIGH); // Assuming LSM_CS is the CS pin for your LSM sensor
//   digitalWrite(LSM9DS0_CSXM, HIGH); // Assuming LSM_CS is the CS pin for your LSM sensor
//   SPI.endTransaction();

//  adaFruitLSM.printMenu();
//   // Then wait for any serial data to come in:
//   while (!Serial.available())
//     ;
//   // Once serial data is received, call parseMenu to act on it:
//   adaFruitLSM.parseMenu(Serial.read());
}

