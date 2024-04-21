
#ifndef BLUEFRUITLE_H
#define BLUEFRUITLE_H


#include <Adafruit_BluefruitLE_SPI.h>
#include "../DeviceRoutine.h"

// Setup for Bluetooth LE Friend
#define BLUEFRUIT_SPI_CS               7
#define BLUEFRUIT_SPI_IRQ              6
#define BLUEFRUIT_SPI_RST              3

class BluefruitLE : public DeviceRoutine {
    Adafruit_BluefruitLE_SPI ble; // Declare the BLE member variable

public:
    // Constructor with member initializer list
    BluefruitLE() : ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST) {}

    virtual void init() override; // Initialize the Bluefruit LE module
    virtual void loop() override; // Loop method to be executed repeatedly
    void checkBLE() {}; // Check for BLE messages and handle them
};

#endif // BLUEFRUITLE_H