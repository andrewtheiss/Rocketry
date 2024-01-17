#ifndef LSM_SENSOR_H
#define LSM_SENSOR_H

// The SFE_LSM9DS0 requires both the SPI and Wire libraries.
#include <SPI.h> // Included for SFE_LSM9DS0 library
#include <Wire.h>
#include <SFE_LSM9DS0.h>


///////////////////////
//     SPI Setup     //
///////////////////////
#define LSM9DS0_CSG  10  // CSG connected to Arduino pin 9
#define LSM9DS0_CSXM 9 // CSXM connected to Arduino pin 10


///////////////////////////////
// Interrupt Pin Definitions //
///////////////////////////////
const byte INT1XM = 2; // INT1XM tells us when accel data is ready
const byte INT2XM = 3; // INT2XM tells us when mag data is ready
const byte DRDYG = 4;  // DRDYG tells us when gyro data is ready



class LSMSensor {
public:
    LSMSensor(); // Constructor
    void init();
    void printAccel();
    void printGyro();
    void printMag();
    void streamAll();
    void setScale();
    void setRaw();
    void setODR();
    float calcHeading(float hx, float hy, float hz);
    void printMenu();
    void parseMenu(char c);

private:
    LSM9DS0 dof;
    boolean printRaw;
    const byte INT1XM = 2;
    const byte INT2XM = 3;
    const byte DRDYG = 4;
};
#endif // LSM_SENSOR_H  


// https://github.com/kriswiner/LSM9DS0/blob/master/LSM9DS0_AHRS.ino
/*****************************************************************
LSM9DS0_SerialMenus.ino
SFE_LSM9DS0 Library Example Code: Interact With Serial Menus
Jim Lindblom @ SparkFun Electronics
Original Creation Date: February 14, 2014 (Happy Valentines Day!)
https://github.com/sparkfun/LSM9DS0_Breakout

This Arduino sketch is a demo of all things SEF_LSM9DS0 library.
Once you attach all hardware, and upload the sketch, open your
Serial monitor at 115200 BPS. Follow the menu prompts to either:
    1) Stream readings from the accelerometer.
    2) Stream readings from the gyroscope.
    3) Stream readings from the magnetometer.
    4) Set the scales of each sensor (e.g. +/-4g, 500DPS, 8Gs)
    5) Switch to/from calculated or raw data (e.g. ADC ticks or
        g's, DPS, and Gs)
    6) Set the output data rate of each sensor.

Hardware setup: This library supports communicating with the
LSM9DS0 over either I2C or SPI. In addition to those wires, this
sketch demos how to use the interrupts. Here's what the I2C setup
looks like:
    LSM9DS0 --------- Arduino
     CSG ------------- NONE (Pulled HIGH [indicates I2C mode])
     CSXM ------------ NONE (Pulled HIGH [indicates I2C mode])
     SDOG ------------ NONE (Pulled HIGH [sets I2C address])
     SDOXM ----------- NONE (Pulled HIGH [sets I2C address])
     SCL ---------- SCL (A5 on older 'Duinos')
     SDA ---------- SDA (A4 on older 'Duinos')
     VDD ------------- 3.3V
     GND ------------- GND
     DEN ------------- NONE (Not used in this example)
     INTG ------------ NONE (Not used in this example)
     DRDYG ------------ 4 (Could be any digital pin)
     INT1XM ----------- 3 (Could be any digital pin)
     INT2XM ----------- 2 (Could be any digital pin)

The LSM9DS0 has a maximum voltage of 3.6V. Make sure you power it
off the 3.3V rail! And either use level shifters between SCL
and SDA or just use a 3.3V Arduino Pro.   

Development environment specifics:
    IDE: Arduino 1.0.5
    Hardware Platform: Arduino Pro 3.3V/8MHz
    LSM9DS0 Breakout Version: 1.0

This code is beerware; if you see me (or any other SparkFun 
employee) at the local, and you've found our code helpful, please 
buy us a round!

Distributed as-is; no warranty is given.
*****************************************************************/

///////////////////////
// Example I2C Setup //
///////////////////////
// SDO_XM and SDO_G are both grounded, therefore our addresses are:
// #define LSM9DS0_XM  0x1D // Would be 0x1E if SDO_XM is LOW
// #define LSM9DS0_G   0x6B // Would be 0x6A if SDO_G is LOW
// // Create an instance of the LSM9DS0 library called `dof` the
// // parameters for this constructor are:
// // [SPI or I2C Mode declaration], [gyro I2C address], [xm I2C address]
// LSM9DS0 dof(MODE_I2C, LSM9DS0_G, LSM9DS0_XM);
