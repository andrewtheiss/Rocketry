// Create the cpp functions for this:

// public:
//     BluefruitLE(); // Constructor
//     virtual void init() override; // Initialize the Bluefruit LE module
//     virtual void loop() override; // Loop method to be executed repeatedly
//     void checkBLE(); // Check for BLE messages and handle them
// };

#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#include "BluefruitLE.h"

void BluefruitLE::init() {
    Serial.begin(115200);
    Serial.println(F("Initializing the Bluefruit LE module..."));

    if ( !ble.begin(VERBOSE_MODE) )
    {
        error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
    }
    Serial.println( F("OK!") );


    /* Disable command echo from Bluefruit */
    ble.echo(false);

    Serial.println("Requesting Bluefruit info:");
    /* Print Bluefruit information */
    ble.info();

    /* Change the device name to make it easier to find */
    Serial.println(F("Setting device name to 'Bluefruit Keyboard': "));
    if (! ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=Bluefruit Keyboard" )) ) {
        Serial.println("Could not set device name?");
    }

    /* Enable HID Service */
    Serial.println(F("Enable HID Service (including Keyboard): "));
    if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
    {
        if ( !ble.sendCommandCheckOK(F( "AT+BleHIDEn=On" ))) {
        Serial.println("Could not enable Keyboard HID");
        }
    }else
    {
        if (! ble.sendCommandCheckOK(F( "AT+BleKeyboardEn=On"  ))) {
        Serial.println("Could not enable Keyboard En");
        }
    }

    /* Add or remove service requires a reset */
    Serial.println(F("Performing a SW reset (service changes require a reset): "));
    if (! ble.reset() ) {
        Serial.println("Couldnt reset");
    }

    Serial.println();
    Serial.println(F("Go to your phone's Bluetooth settings to pair your device"));
    Serial.println(F("then open an application that accepts keyboard input"));

    Serial.println();
    Serial.println(F("Enter the character(s) to send:"));
    Serial.println(F("- \\r for Enter"));
    Serial.println(F("- \\n for newline"));
    Serial.println(F("- \\t for tab"));
    Serial.println(F("- \\b for backspace"));

    Serial.println();

}

void BluefruitLE::loop() {
  // Display prompt
  Serial.print(F("keyboard > "));

  // Check for user input and echo it back if anything was found
  char keys[BUFSIZE+1];
  getUserInput(keys, BUFSIZE);

  Serial.print("\nSending ");
  Serial.println(keys);

  ble.print("AT+BleKeyboard=");
  ble.println(keys);

  if( ble.waitForOK() )
  {
    Serial.println( F("OK!") );
  }else
  {
    Serial.println( F("FAILED!") );
  }
}

void BluefruitLE::getUserInput(char buffer[], uint8_t maxSize)
{
  memset(buffer, 0, maxSize);
  while( Serial.available() == 0 ) {
    delay(1);
  }

  uint8_t count=0;

  do
  {
    count += Serial.readBytes(buffer+count, maxSize);
    delay(2);
  } while( (count < maxSize) && !(Serial.available() == 0) );
}