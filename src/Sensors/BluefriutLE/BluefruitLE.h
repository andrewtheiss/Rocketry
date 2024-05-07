
#ifndef BLUEFRUITLE_H
#define BLUEFRUITLE_H

#include <Arduino.h>
#include "BluefruitConfig.h"
#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_SPI.h>
#include "Adafruit_BluefruitLE_UART.h"
#include <SPI.h>
#include "./PacketParser.h"
#include "../DeviceRoutine.h"

// Setup for Bluetooth LE Friend
#define BLUEFRUIT_SPI_CS               7
#define BLUEFRUIT_SPI_IRQ              6
#define BLUEFRUIT_SPI_RST              3
#define BLE_READPACKET_TIMEOUT         500 // Timeout for reading packets in milliseconds
#define BLUEFRUIT_SPI_SCK              13
#define BLUEFRUIT_SPI_MISO             12
#define BLUEFRUIT_SPI_MOSI             11
class BluefruitLE : public DeviceRoutine {
    Adafruit_BluefruitLE_SPI ble; // Declare the BLE member variable
    uint8_t packetBuffer[20]; // Buffer to store incoming packets

    void error(const __FlashStringHelper* err) {
      Serial.println(err);
      while (1);
    }
    String byteToBinary(uint8_t byte); 
    void printPacket(const uint8_t* buffer, uint8_t bufferSize);
    bool sendNVMReadCommand(uint32_t address, uint16_t length);


public:
    // Constructor with member initializer list
    // BluefruitLE() : ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST) {}
    BluefruitLE() : ble(BLUEFRUIT_SPI_SCK,BLUEFRUIT_SPI_MISO, BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST) {
      state currentState = PREPARE_TO_ARM;
    }
    virtual void init() override; // Initialize the Bluefruit LE module
    virtual void loop() override; // Loop method to be executed repeatedly
    void checkBLE() {}; // Check for BLE messages and handle them
    int getUserInput(uint8_t buffer[], uint8_t maxSize);
    enum state {
      PREPARE_TO_ARM,
      ARMED,
      FIRED
    };
    int getCurrentState();
    void setPrepareToArm() {
      currentState = PREPARE_TO_ARM;
    }
    
    private:
      state currentState;
    
};

#endif // BLUEFRUITLE_H


// WARNING - must add delay in read/write
// https://forum.pjrc.com/index.php?threads/teensy-3-6-vs-arduino-issues-with-adafruit-bluefruit-le-spi-friend.45440/page-2
// #define SPI_CS_ENABLE()           digitalWrite(m_cs_pin, LOW)
// #define SPI_CS_DISABLE()          digitalWrite(m_cs_pin, HIGH)
// #define SPI_CS_ENABLE() digitalWrite(m_cs_pin, LOW);delayMicroseconds(1)
// #define SPI_CS_DISABLE() delayMicroseconds(1);digitalWrite(m_cs_pin, HIGH)