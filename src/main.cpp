#include <LSMSensor.h>
#include <Adafruit_BluefruitLE_SPI.h>

// Setup for Bluetooth LE Friend
#define BLUEFRUIT_SPI_CS               7
#define BLUEFRUIT_SPI_IRQ              6
#define BLUEFRUIT_SPI_RST              3

LSMSensor adaFruitLSM;

void setup()
{
  adaFruitLSM.init();
  Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);
}

void loop()
{
  // Print the control menu:
  adaFruitLSM.printMenu();
  // Then wait for any serial data to come in:
  while (!Serial.available())
    ;
  // Once serial data is received, call parseMenu to act on it:
  adaFruitLSM.parseMenu(Serial.read());

}
