// Create the cpp functions for this:

// public:
//     BluefruitLE(); // Constructor
//     virtual void init() override; // Initialize the Bluefruit LE module
//     virtual void loop() override; // Loop method to be executed repeatedly
//     void checkBLE(); // Check for BLE messages and handle them
// };

#define MINIMUM_FIRMWARE_VERSION "0.6.6"
#include "BluefruitLE.h"

void BluefruitLE::getData(char data[]) {

}

const char* BluefruitLE::getName() {
    return "Bluefruit";
}

void BluefruitLE::init()
{
    Serial.begin(115200);
    Serial.println(F("Initializing the Bluefruit LE module..."));

    if (!ble.begin(VERBOSE_MODE))
    {
        error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
    }
    Serial.println(F("OK!"));

    /* Disable command echo from Bluefruit */
    ble.echo(false);

    Serial.println("Requesting Bluefruit info:");
    /* Print Bluefruit information */
    ble.info();

    /* Change the device name to make it easier to find */
    Serial.println(F("Setting device name to 'Bluefruit Keyboard': "));
    if (!ble.sendCommandCheckOK(F("AT+GAPDEVNAME=Bluefruit Keyboard")))
    {
        Serial.println("Could not set device name?");
    }

    /* Enable HID Service */
    Serial.println(F("Enable HID Service (including Keyboard): "));
    // if (ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION))
    // {
    //     if (!ble.sendCommandCheckOK(F("AT+BleHIDEn=On")))
    //     {
    //         Serial.println("Could not enable Keyboard HID");
    //     }
    // }
    // else
    // {
        if (!ble.sendCommandCheckOK(F("AT+BleKeyboardEn=1")))
        {
            Serial.println("Could not enable Keyboard En");
        }
    //}
    if (!ble.sendCommandCheckOK(F("AT+BLEKEYBOARD=http://www.adafruit.com\r\n")))
    {
        Serial.println("Could not send keyboard command?");
    }

    /* Add or remove service requires a reset */
    Serial.println(F("Performing a SW reset (service changes require a reset): "));
    if (!ble.reset())
    {
        Serial.println("Couldnt reset");
    }

    // // Set Bluefruit to DATA mode
    // Serial.println( F("Switching to DATA mode!") );
    // ble.setMode(BLUEFRUIT_MODE_DATA);

    // /* Add or remove service requires a reset */
    // Serial.println(F("Performing a SW reset (service changes require a reset): "));
    // if (! ble.reset() ) {
    //     Serial.println("Couldnt reset");
    // }
    bool waitToArm = false;
    if (waitToArm) {
        // Set Bluefruit to DATA mode
        Serial.println( F("Switching to DATA mode!") );
        ble.setMode(BLUEFRUIT_MODE_DATA);

        Serial.println(F("******************************"));
        Serial.println(F("Performing a SW reset (service changes require a reset): "));
        if (!ble.reset())
        {
            Serial.println("Couldnt reset");
        }
        String arm = "00000110";
        uint8_t len = 0;
        Serial.println("Waiting to arm");
        currentState = PREPARE_TO_ARM;
        while (!byteToBinary(len).equals(arm)) {
            /* Wait for new data to arrive */
            len = readPacket(&ble, BLE_READPACKET_TIMEOUT/50);
            // if (len == 0) {
            //     //Serial.println("No data received or error in receiving data");
            // } else {
            //     Serial.println("DATA received or error in receiving data: " +byteToBinary(len));
            // }
        }
        currentState = FIRE;
        Serial.println("Armed and fired");
    }
}

bool BluefruitLE::sendNVMReadCommand(uint32_t address, uint16_t length)
{
    char command[50];
    sprintf(command, "AT+NVMREADRAW=%lu,%u\r\n", address, length); // Create command string
    return ble.sendCommandCheckOK(command); // Send command and check for "OK"
}

int BluefruitLE::getCurrentState() {
    return currentState;
}

void BluefruitLE::loop()
{
    // Serial.begin(115200);

    // delay(4200);

    // uint32_t nvm_address = 0x0000;  // Example address
    // uint16_t nvm_length = 16;       // Example length to read

    // if (!sendNVMReadCommand(nvm_address, nvm_length))
    // {
    //     Serial.println("Failed to send NVM read command");
    // }
    // else
    // {
    //     // Wait for data to be available
    //     uint8_t buffer[64]; // Adjust buffer size as needed based on maximum expected response size
    //     int len = getUserInput(buffer, sizeof(buffer)); // Read response into buffer
    //     if (len > 0) {
    //         printPacket(buffer, len); // Print the raw NVM data
    //     } else {
    //         Serial.println("No data received or error in receiving data");
    //     }
    // }
    // delay(4200);
    // // // Usage
    // // uint8_t data = 0xAB; // Example data byte
    // // Serial.println(byteToBinary(data)); // Prints: 10101011
    // /* Wait for new data to arrive */
    // // uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT);
    // // uint8_t len = ble.peek();
    // // Serial.println(len);
    // Serial.begin(9600);
}

int BluefruitLE::getUserInput(uint8_t buffer[], uint8_t maxSize)
{
    memset(buffer, 0, maxSize);
    uint8_t count = 0;
    while (Serial.available() == 0 && count == 0)
    {
        delay(1); // Small delay to wait for data
    }

    count = Serial.readBytes((char*)buffer, maxSize); // Cast buffer to char* for readBytes
    return count; // Return the number of bytes read
}

String BluefruitLE::byteToBinary(uint8_t byte)
{
    String result = "";
    for (int i = 7; i >= 0; i--)
    {
        result += (byte & (1 << i)) ? "1" : "0";
    }
    return result;
}

void BluefruitLE::printPacket(const uint8_t *buffer, uint8_t bufferSize)
{
    for (uint8_t i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}