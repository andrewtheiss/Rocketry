
#include "LSMSensor.h"
#include <Arduino.h>

LSMSensor::LSMSensor() : dof(MODE_SPI, LSM9DS0_CSG, LSM9DS0_CSXM) { 
    // A boolean to keep track of whether we're printing raw (ADC)
    // or calculated (g's, DPS, Gs) sensor data:
    printRaw = true;
}

// LSMSensor::LSMSensor() : dof(MODE_SPI, LSM9DS0_CSG, LSM9DS0_CSXM) { 
//     // A boolean to keep track of whether we're printing raw (ADC)
//     // or calculated (g's, DPS, Gs) sensor data:
//     printRaw = true;
// }


void LSMSensor::loop() {
  
}

// Function to format accelerometer data
char* LSMSensor::getFormattedAcceleration() {
    static char buffer[128]; // Static buffer for data formatting

    dof.readAccel(); // Assume dof.readAccel() updates ax, ay, az

    // Directly format into the buffer without using sprintf, std::to_chars, etc.
    int ax = dof.ax, ay = dof.ay, az = dof.az; // Assuming these are integer values
    float ax_g = dof.calcAccel(ax), ay_g = dof.calcAccel(ay), az_g = dof.calcAccel(az); // Assuming these are float values
    int len = 0; // Variable to keep track of the buffer length

    // Manually convert integers to strings and concatenate to buffer
    len += floatToString(ax_g, buffer + len, 5);
    buffer[len++] = ',';
    buffer[len++] = ' ';
    len += floatToString(ay_g, buffer + len, 5);
    buffer[len++] = ',';
    buffer[len++] = ' ';
    len += floatToString(az_g, buffer + len, 5);
    buffer[len] = '\0'; // Null-terminate the string
    
    return buffer; // Return the static buffer
}


void LSMSensor::init() {
 Serial.begin(9600); // Initialize serial communication
    
    // Initialize CS pins for both gyro and XM modules
    pinMode(LSM9DS0_CSG, OUTPUT);
    pinMode(LSM9DS0_CSXM, OUTPUT);
    digitalWrite(LSM9DS0_CSG, HIGH); // Deselect the gyro module
    digitalWrite(LSM9DS0_CSXM, HIGH); // Deselect the XM module
    
    // Initialize SPI
    SPI.begin();
    SPI.setDataMode(SPI_MODE3); // The LSM9DS0 typically operates in SPI Mode 3
    SPI.setBitOrder(MSBFIRST);  // Default bit order for SPI
    SPI.setClockDivider(SPI_CLOCK_DIV4); // Adjust as necessary for your microcontroller and sensor speed

    // Sensor-specific initialization
    uint16_t status = dof.begin();
    if (status == 0x49D4) {
        Serial.println("LSM9DS0 initialized successfully");
    } else {
        Serial.print("LSM9DS0 initialization failed, WHO_AM_I's returned: 0x");
        Serial.println(status, HEX);
    }
    setRaw();
}

void LSMSensor::refreshForRead() {
  SPI.setDataMode(SPI_MODE3); // The LSM9DS0 typically operates in SPI Mode 3
  SPI.setBitOrder(MSBFIRST);  // Default bit order for SPI
  SPI.setClockDivider(SPI_CLOCK_DIV4); // Adjust as necessary for your microcontroller and sensor speed

  uint16_t status = dof.begin();
  if (status != 0x49D4) {
    Serial.print("LSM9DS0 initialization failed, WHO_AM_I's returned: 0x");
    Serial.println(status, HEX);
  }
}

// Here's a simple example function to calculate heading based on
// magnetometer readings. This only works when the 9DOF is flat
// (x-axis normal to gravity).
float LSMSensor::calcHeading(float hx, float hy, float hz)
{  
  if (hy > 0)
  {
    return 90 - atan(hx / hy) * 180 / PI;
  }
  else if (hy < 0)
  {
    return 270 - atan(hx / hy) * 180 / PI;
  }
  else // hy = 0
  {
    if (hx < 0) return 180;
    else return 0;
  }
}

// This function will print all data from all sensors at once.
// It'll wait until every sensor interrupt triggers before
// printing.
void LSMSensor::streamAll()
{
  if ((digitalRead(INT2XM)) && (digitalRead(INT1XM)) &&
      (digitalRead(DRDYG)))
  {
    printAccel();
    printGyro();
    printMag();
  }
}