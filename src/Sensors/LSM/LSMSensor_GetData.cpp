#include "LSMSensor.h"

void LSMSensor::getData(char data[]) {
    refreshForRead();
    const char* formattedAcceleration = getFormattedAcceleration();

     // Parse the formatted acceleration string to extract ax, ay, az
    float ax, ay, az;
    sscanf(formattedAcceleration, "A: %f, %f, %f", &ax, &ay, &az);

    // Calculate the magnitude of the overall g-force
    float magnitude = sqrt(ax * ax + ay * ay + az * az);

    // Format the data string
    snprintf(data, 100, "%s", formattedAcceleration);
    //printAccel();
}

const char* LSMSensor::getName() {
    return "LSMAccel";
}
