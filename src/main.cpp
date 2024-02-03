#include <LSMSensor.h>
#include <Adafruit_BluefruitLE_SPI.h>
#include "TouchScreenTFT.h"

// Setup for Bluetooth LE Friend
#define BLUEFRUIT_SPI_CS               7
#define BLUEFRUIT_SPI_IRQ              6
#define BLUEFRUIT_SPI_RST              3

#define SOLENOID_PIN 21

TouchScreenTFT touchScreen;
LSMSensor adaFruitLSM;

#define REMOTE_DETONATION_SAFE_STATE false
#define REMOTE_DETONATION_ARMED_STATE true
bool solenoidState = REMOTE_DETONATION_SAFE_STATE;
void setup()
{
  touchScreen.init();
  adaFruitLSM.init();
  Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);
  digitalWrite(SOLENOID_PIN, REMOTE_DETONATION_SAFE_STATE);
  pinMode(SOLENOID_PIN, OUTPUT); // Initialize the solenoid pin as an output
}

void toggleSolenoid()
{
  Serial.println("Toggling solenoid");
  digitalWrite(SOLENOID_PIN, REMOTE_DETONATION_ARMED_STATE);
  delay(5000);
  digitalWrite(SOLENOID_PIN, REMOTE_DETONATION_SAFE_STATE);
  Serial.println("Toggling solenoid back off");
}

void loop()
{
  // Check for touch events:
  unsigned int touchCommand = touchScreen.checkTouch();
  switch (touchCommand)
  {
  case 1:
    toggleSolenoid();
    break;
    default:
    break;
  }
  // // Print the control menu:
  // adaFruitLSM.printMenu();
  // // Then wait for any serial data to come in:
  // while (!Serial.available())
  //   ;
  // // Once serial data is received, call parseMenu to act on it:
  // adaFruitLSM.parseMenu(Serial.read());

}

