#include "Flight.h"
#include <Arduino.h>

void Flight::LoopDataRecording() {
    
    // Add another datapoint to the sensors:
    uint32_t currentTime = m_pTimer->elapsedMilliseconds();
    if (currentTime - lastSampleTime >= 500) {  // Sample every 1/2 second
        lastSampleTime = currentTime;
        
        double temperature, pressure, latestAltitude;
        if (m_bmpSensor->startTemperature() && m_bmpSensor->getTemperature(temperature) && m_bmpSensor->startPressure() && m_bmpSensor->getPressure(pressure, temperature)) {
            float currentElevation = m_bmpSensor->altitude(pressure, 1013.25); // Assuming sea-level standard pressure is 1013.25 mb
            elevationSamples[sampleIndex] = currentElevation;
        }

        // float accelX, accelY, accelZ;
        // m_lsmSensor->getAcceleration(accelX, accelY, accelZ);
        // float currentAcceleration = sqrt(accelX*accelX + accelY*accelY + accelZ*accelZ);
        
        // accelerationSamples[sampleIndex] = currentAcceleration;
        
        sampleIndex++;
    }


    if (sampleIndex >= SAMPLE_SIZE) {
        getSensorIdleAverages();
    }
}

// Once we have the idle values, average them and, move to IDLE_LAUNCH_READY state
void Flight::getSensorIdleAverages() {


    // Calculate averages
    float sumElevation = 0, sumAcceleration = 0;
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        sumElevation += elevationSamples[i];
        sumAcceleration += accelerationSamples[i];
    }
    averageElevation = sumElevation / SAMPLE_SIZE;
    averageAcceleration = sumAcceleration / SAMPLE_SIZE;
    
    // Move to IDLE_LAUNCH_READY state
    setStatus(IDLE_LAUNCH_READY);
    Serial.println("Idle averages calculated. Ready for launch.");
    Serial.print("Average Elevation: ");
    Serial.println(averageElevation);
    Serial.print("Average Acceleration: ");
    Serial.println(averageAcceleration);
    // TODO -log this instead of print
}

void Flight::LoopIdleLaunchReady() {
    
    double temperature, pressure, latestAltitude;
    if (m_bmpSensor->startTemperature() && m_bmpSensor->getTemperature(temperature) && m_bmpSensor->startPressure() && m_bmpSensor->getPressure(pressure, temperature)) {
        float currentElevation = m_bmpSensor->altitude(pressure, 1013.25); // Assuming sea-level standard pressure is 1013.25 mb
        
        if (currentElevation - averageElevation > LAUNCH_HEIGHT_THRESHOLD) {
            setStatus(IN_FLIGHT_ASCENT);
            Serial.println("Launch detected by altitude!");
            return;
        }

    }
    // float accelX, accelY, accelZ;
    // m_lsmSensor->getAcceleration(accelX, accelY, accelZ);
    // float currentAcceleration = sqrt(accelX*accelX + accelY*accelY + accelZ*accelZ);
    
    // if (currentAcceleration > LAUNCH_G_THRESHOLD) {
    //     if (launchDetectionStartTime == 0) {
    //         launchDetectionStartTime = millis();
    //     } else if (millis() - launchDetectionStartTime >= 1000) {  // 1 second of consecutive high G force
    //         setStatus(IN_FLIGHT_ASCENT);
    //         Serial.println("Launch detected by acceleration!");
    //         return;
    //     }
    // } else {
    //     launchDetectionStartTime = 0;
    // }
    

}

void Flight::LoopAscent() {
    // Placeholder function for ascent phase
    Serial.println("In ascent phase");
}

void Flight::LoopAfterApogee() {
    // Placeholder function for after apogee phase
    Serial.println("After apogee phase");
}

void Flight::LoopFlightComplete() {
    // Placeholder function for flight complete phase
    Serial.println("Flight complete phase");
}