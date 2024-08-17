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

// Once we have the idle values, average them and, move to CALIBRATED_LAUNCH_READY state
void Flight::getSensorIdleAverages() {


    // Calculate averages
    float sumElevation = 0, sumAcceleration = 0;
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        sumElevation += elevationSamples[i];
        sumAcceleration += accelerationSamples[i];
    }
    averageStartElevation = sumElevation / SAMPLE_SIZE;
    averageStartAcceleration = sumAcceleration / SAMPLE_SIZE;
    
    // Verify the averages
    // TODO -log this instead of print
    Serial.println("Idle averages calculated. Ready for launch.");
    Serial.print("Average Elevation: ");
    Serial.println(averageStartElevation);
    Serial.print("Average Acceleration: ");
    Serial.println(averageStartAcceleration);
    
    // Sensor Average Complete!
    // Move to CALIBRATED_LAUNCH_READY state
    setStatus(CALIBRATED_LAUNCH_READY);
}

void Flight::LoopIdleLaunchReady() {
    
    double temperature, pressure;
    if (!elevationGainDetected) {
        if (m_bmpSensor->startTemperature() && m_bmpSensor->getTemperature(temperature) && m_bmpSensor->startPressure() && m_bmpSensor->getPressure(pressure, temperature)) {
            float currentElevation = m_bmpSensor->altitude(pressure, 1013.25); // Assuming sea-level standard pressure is 1013.25 mb
            
            if (currentElevation - averageStartElevation > LAUNCH_HEIGHT_THRESHOLD) {
                elevationGainDetected = true;
                Serial.println("Launch detected by altitude!");
                // print current and average elevation
                Serial.print("Current Elevation: ");
                Serial.println(currentElevation);
                Serial.print("Average Elevation: ");
                Serial.println(averageStartElevation);
            }
        }
    }
    
    
    if (!highGDetected) {
        float currentAccelerationMagnitude = m_lsmSensor->getAccelerationMagnitude();
        if (currentAccelerationMagnitude > (averageStartAcceleration + LAUNCH_G_THRESHOLD)) {
            if (launchDetectionStartTime == 0) {
                launchDetectionStartTime = m_pTimer->elapsedMilliseconds();
                // print high g detected and start time
                Serial.println("High G detected");
                Serial.print(currentAccelerationMagnitude);
                Serial.print("   - Start Time: ");
                Serial.println(launchDetectionStartTime);

            } else if (m_pTimer->elapsedMilliseconds() - launchDetectionStartTime >= 1000) {  // 1 second of consecutive high G force
                highGDetected = true;
                // print start time and launch detected by acceleration
                Serial.print("Start Time: ");
                Serial.println(launchDetectionStartTime);
                Serial.print("TimeNow: ");  
                Serial.println(m_pTimer->elapsedMilliseconds());
                Serial.print("acceleration: ");
                Serial.println(currentAccelerationMagnitude);
                Serial.println("Launch detected by acceleration!");
            }
        } else {
            Serial.println("High G ended");
            Serial.print(currentAccelerationMagnitude);
            Serial.print("   - End Time: ");
            launchDetectionStartTime = 0;
        }
    }
    
    if (highGDetected && elevationGainDetected) {
        setStatus(IN_FLIGHT_ASCENT);
    }
}

void Flight::LoopAscent() {

    // Wait to check for height and g-force
    if (!timerForApogeeFinished) {
        timerForApogeeCheckToBegin = m_pTimer->elapsedMilliseconds();
        if (timerForApogeeCheckToBegin >= 3000) {
            timerForApogeeFinished = true;
        }
    } else {
        double temperature, pressure;
        if (m_bmpSensor->startTemperature() && m_bmpSensor->getTemperature(temperature) && m_bmpSensor->startPressure() && m_bmpSensor->getPressure(pressure, temperature)) {
            float currentElevation = m_bmpSensor->altitude(pressure, 1013.25); // Assuming sea-level standard pressure is 1013.25 mb
            

            // Subtract the oldest value if the buffer is full
            if (count == size) {
                sum -= values[current_index] * (count - size + 1);
                weight_sum -= count - size + 1;
            } else {
                count++;
            }

            // Add the new value
            values[current_index] = currentElevation;
            sum += currentElevation * count;
            weight_sum += count;

            // Calculate weighted moving average
            float wma = sum / weight_sum;


            // Update max elevation
            if (wma > maxElevation) {
                maxElevation = wma;
            }

            // Check for apogee
            if (maxElevation - wma > APOGEE_THRESHOLD) {
                setStatus(IN_FLIGHT_APOGEE);
                return; // Exit the function if apogee is detected
            }

            // Print debug information
            Serial.print("Current Elevation: ");
            Serial.print(currentElevation);
            Serial.print(" WMA: ");
            Serial.print(wma);
            Serial.print(" Max Elevation: ");
            Serial.print(maxElevation);
            Serial.print(" Difference: ");
            Serial.println(maxElevation - wma);

            // Move to the next position
            current_index = (current_index + 1) % size;
        }
    }

    // Check for apogee using G-force
    // G-force check
    float gForceMagnitude =  m_lsmSensor->getAccelerationMagnitude();
    if (gForceMagnitude < G_FORCE_THRESHOLD) {
        lowGForceCount++;
        if (lowGForceCount >= APOGEE_G_FORCE_COUNT) {
            setStatus(IN_FLIGHT_APOGEE);
            return; // Exit the function if apogee is detected
        }
    } else {
        lowGForceCount = 0;
    }
}

void Flight::LoopAfterApogee() {
    double temperature, pressure;
    if (m_bmpSensor->startTemperature() && m_bmpSensor->getTemperature(temperature) && m_bmpSensor->startPressure() && m_bmpSensor->getPressure(pressure, temperature)) {
        float currentElevation = m_bmpSensor->altitude(pressure, 1013.25); // Assuming sea-level standard pressure is 1013.25 mb
        
        // From Jackson      
        // if (currentStatus == APOGEE && bmp180.altitude(newPressure, avgPadPressure) < 275) {
        //     setStatus(MAIN_CHUTE);
        // }
        if ((currentElevation - averageStartElevation) < 275 ) {
            setStatus(IN_FLIGHT_MAIN_CHUTE_DEPLOYED);
        }
    }

}

void Flight::LoopFlightComplete() {
    // Placeholder function for flight complete phase
    //Serial.println("Flight complete phase");
}