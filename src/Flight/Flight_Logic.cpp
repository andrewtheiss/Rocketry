#include "Flight.h"
#include <Arduino.h>

void Flight::LoopDataRecording() {
    
    // Add another datapoint to the sensors:
    uint32_t currentTime = m_pTimer->elapsedMilliseconds();
    if (currentTime - lastSampleTime >= 500) {  // Sample every 1/2 second
    
        Serial.println("Sample Time" + currentTime);
        lastSampleTime = currentTime;
        
        // Sample the Barometer elevation
        double temperature, pressure;
        if (m_bmpSensor->startTemperature() && m_bmpSensor->getTemperature(temperature) && m_bmpSensor->startPressure() && m_bmpSensor->getPressure(pressure, temperature)) {
            float currentElevation = m_bmpSensor->altitude(pressure, 1013.25); // Assuming sea-level standard pressure is 1013.25 mb
            elevationSamples[sampleIndex] = currentElevation;

            // print float
            Serial.print("Elevation: ");
            Serial.println(currentElevation);
        }


        // Sample the Accelerometer data
        float currentAccelerationMagnitude = m_lsmSensor->getAccelerationMagnitude();
        Serial.print("Acceleration: ");
        Serial.println(currentAccelerationMagnitude);     
        accelerationSamples[sampleIndex] = currentAccelerationMagnitude;
        
        sampleIndex++;
    }

    // After SAMPLE_SIZE samples, calculate the average values
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
    
    // Verify the averages
    // TODO -log this instead of print
    Serial.println("Idle averages calculated. Ready for launch.");
    Serial.print("Average Elevation: ");
    Serial.println(averageElevation);
    Serial.print("Average Acceleration: ");
    Serial.println(averageAcceleration);
    
    // Sensor Average Complete!
    // Move to IDLE_LAUNCH_READY state
    setStatus(IDLE_LAUNCH_READY);
}

void Flight::LoopIdleLaunchReady() {
    
    double temperature, pressure, latestAltitude;
    if (m_bmpSensor->startTemperature() && m_bmpSensor->getTemperature(temperature) && m_bmpSensor->startPressure() && m_bmpSensor->getPressure(pressure, temperature)) {
        float currentElevation = m_bmpSensor->altitude(pressure, 1013.25); // Assuming sea-level standard pressure is 1013.25 mb
        
        if (currentElevation - averageElevation > LAUNCH_HEIGHT_THRESHOLD) {
            Serial.println("Launch detected by altitude!");
            // print current and average elevation
            Serial.print("Current Elevation: ");
            Serial.println(currentElevation);
            Serial.print("Average Elevation: ");
            Serial.println(averageElevation);
            return;
        }

    }
    
    float currentAccelerationMagnitude = m_lsmSensor->getAccelerationMagnitude();
    if (currentAccelerationMagnitude > (averageAcceleration + LAUNCH_G_THRESHOLD)) {
        if (launchDetectionStartTime == 0) {
            launchDetectionStartTime = m_pTimer->elapsedMilliseconds();
            // print high g detected and start time
            Serial.println("High G detected");
            Serial.print(currentAccelerationMagnitude);
            Serial.print("   - Start Time: ");
            Serial.println(launchDetectionStartTime);

        } else if (m_pTimer->elapsedMilliseconds() - launchDetectionStartTime >= 1000) {  // 1 second of consecutive high G force
            // print start time and launch detected by acceleration
            Serial.print("Start Time: ");
            Serial.println(launchDetectionStartTime);
            Serial.print("TimeNow: ");  
            Serial.println(m_pTimer->elapsedMilliseconds());
            Serial.print("acceleration: ");
            Serial.println(currentAccelerationMagnitude);
            Serial.println("Launch detected by acceleration!");
            return;
        }
    } else {
        launchDetectionStartTime = 0;
    }
    
    if (highGDetected && elevationGainDetected) {
        setStatus(IN_FLIGHT_ASCENT);
    }
}

void Flight::LoopAscent() {
    // Placeholder function for ascent phase
    //Serial.println("In ascent phase");
}

void Flight::LoopAfterApogee() {
    // Placeholder function for after apogee phase
    //Serial.println("After apogee phase");
}

void Flight::LoopFlightComplete() {
    // Placeholder function for flight complete phase
    //Serial.println("Flight complete phase");
}