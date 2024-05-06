// Create the cpp functions for this:

// public:
//     BluefruitLE(); // Constructor
//     virtual void init() override; // Initialize the Bluefruit LE module
//     virtual void loop() override; // Loop method to be executed repeatedly
//     void checkBLE(); // Check for BLE messages and handle them
// };

#include "BluefruitLE.h"

void BluefruitLE::init() {
    Serial.begin(9600);
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


}

void BluefruitLE::loop() {
  // Display command prompt
  Serial.print(F("AT > "));

  // Check for user input and echo it back if anything was found
  char command[BUFSIZE+1];
  getUserInput(command, BUFSIZE);

  // Send command
  ble.println(command);

  // Check response status
  ble.waitForOK();
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