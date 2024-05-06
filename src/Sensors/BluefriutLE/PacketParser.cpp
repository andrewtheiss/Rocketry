#include "PacketParser.h"

uint8_t packetbuffer[READ_BUFSIZE+1]; // Define the buffer

float parsefloat(uint8_t *buffer) {
    float f;
    memcpy(&f, buffer, 4);
    return f;
}

void printHex(const uint8_t *data, const uint32_t numBytes) {
    uint32_t szPos;
    for (szPos = 0; szPos < numBytes; szPos++) {
        Serial.print(F("0x"));
        if (data[szPos] <= 0xF) {
            Serial.print(F("0"));
        }
        Serial.print(data[szPos], HEX);
        if ((numBytes > 1) && (szPos != numBytes - 1)) {
            Serial.print(F(" "));
        }
    }
    Serial.println();
}

uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout) {
    uint16_t origtimeout = timeout, replyidx = 0;
    memset(packetbuffer, 0, READ_BUFSIZE);

    while (timeout--) {
        if (replyidx >= 20) break;
        while (ble->available()) {
            char c =  ble->read();
            if (c == '!') {
                replyidx = 0;
            }
            packetbuffer[replyidx] = c;
            replyidx++;
            timeout = origtimeout;
        }
        if (timeout == 0) break;
        delay(1);
    }

    packetbuffer[replyidx] = 0;  // null terminate
    if (!replyidx) return 0;  // no data or timeout
    if (packetbuffer[0] != '!') return 0;  // doesn't start with '!' packet beginning

    uint8_t xsum = 0, checksum = packetbuffer[replyidx-1];
    for (uint8_t i = 0; i < replyidx-1; i++) {
        xsum += packetbuffer[i];
    }
    xsum = ~xsum;

    if (xsum != checksum) {
        Serial.print("Checksum mismatch in packet: ");
        printHex(packetbuffer, replyidx+1);
        return 0;
    }
    return replyidx;  // checksum passed!
}
