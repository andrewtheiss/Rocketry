#ifndef _PacketParser_h
#define _PacketParser_h

#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#define PACKET_ACC_LEN                  (15)
#define PACKET_GYRO_LEN                 (15)
#define PACKET_MAG_LEN                  (15)
#define PACKET_QUAT_LEN                 (19)
#define PACKET_BUTTON_LEN               (5)
#define PACKET_COLOR_LEN                (6)
#define PACKET_LOCATION_LEN             (15)
#define READ_BUFSIZE                    (20)

extern uint8_t packetbuffer[READ_BUFSIZE+1];  // Buffer for incoming packets

float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);
uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);

#endif
