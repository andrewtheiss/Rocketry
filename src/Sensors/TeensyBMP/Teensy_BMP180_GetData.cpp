#include "Teensy_BMP180.h"
void Teensy_BMP180::getData(char data[]) {
    double temperature, pressure, latestAltitude;
    if (startTemperature() && getTemperature(temperature) && startPressure() && getPressure(pressure, temperature)) {
        latestAltitude = altitude(pressure, 1013.25); // Assuming sea-level standard pressure is 1013.25 mb
        snprintf(data, 100, "Temperature: %.2f C, Pressure: %.2f mb, Altitude: %.2f meters", temperature, pressure, latestAltitude);
    } else {
        snprintf(data, 100, "Error reading sensor data");
    }
}