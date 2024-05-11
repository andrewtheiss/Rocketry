#include <SD.h>
#include "../../Sensors/DeviceRoutine.h"
#include <map>
#include "../Timer.h"

const char filename[] = "flightData/flightLog00000.txt";

class DataCard {
    private:
        std::map<DeviceRoutine*, File> files;
        File flightData;
        Timer m_timer;
public:
    // Constructor
    DataCard() {}

    void checkDirectory() {
        char directory[] = "flightData"; // Directory to store the logs
        if (!SD.begin(BUILTIN_SDCARD)) {
            Serial.println("The SD card failed to initialize.");
            while (1);
        }
        Serial.println("The SD card has been initialized.");

        // Check if the directory exists, create if not
        if (!SD.exists(directory)) {
            if (SD.mkdir(directory)) {
                Serial.println("Directory created successfully.");
            } else {
                Serial.println("Failed to create directory.");
                return; // Stop further execution if directory creation failed
            }
        }
    }

    void init(Timer& timer) {
        m_timer = timer;

        Serial.println("Trying SD initialization...");
        checkDirectory();
        char baseFilename[] = "flightData/flightLog";
        char fileNumber[6] = "00000";  // Holds the number part of the filename
        char extension[] = ".txt";
        char fullFilename[32];  // Make sure this is large enough to hold the full filename

        if (!SD.begin(BUILTIN_SDCARD)) {
            Serial.println("The SD card failed to initialize.");
            while (1);
        }

        Serial.println("The SD card has been initialized.");

        // Find an unused filename
        for (unsigned int i = 0; i < 100000; i++) {  // Adjust the limit as needed
            sprintf(fileNumber, "%05u", i);  // Generate the next file number in the sequence
            snprintf(fullFilename, sizeof(fullFilename), "%s%s%s", baseFilename, fileNumber, extension);

            if (!SD.exists(fullFilename)) {
                // If the file does not exist, use this filename
                break;
            }
        }

        flightData = SD.open(fullFilename, FILE_WRITE);
        if (!flightData) {
            Serial.println("Failed to open the log file.");
        } else {
            Serial.printf("Log file %s opened for writing.\n", fullFilename);
        }
    }
    
    // Initialize a file for a specific device routine
    bool initFile(DeviceRoutine* device) {
        char baseFilename[32];
        char fileNumber[6] = "00000";  // Holds the number part of the filename
        char extension[] = ".txt";
        char fullFilename[50];  // Make sure this is large enough to hold the full filename

        snprintf(baseFilename, sizeof(baseFilename), "flightData/%s_", device->getName());

        // Find an unused filename
        for (unsigned int i = 0; i < 100000; i++) {  // Adjust the limit as needed
            sprintf(fileNumber, "%05u", i);  // Generate the next file number in the sequence
            snprintf(fullFilename, sizeof(fullFilename), "%s%s%s", baseFilename, fileNumber, extension);

            if (!SD.exists(fullFilename)) {
                // If the file does not exist, use this filename
                break;
            }
        }

        File file = SD.open(fullFilename, FILE_WRITE);
        if (!file) {
            Serial.println("Failed to open the log file.");
            return false;
        } else {
            Serial.printf("Log file %s opened for writing.\n", fullFilename);
            files[device] = file;
            return true;
        }
    }

    
    // Write data to the specified file on the SD card
    bool writeData(const char* data) {
        if (!flightData) {
            // Re-open the file if it's not open
            flightData = SD.open(filename, FILE_WRITE);
            if (!flightData) return false;
        }
        flightData.printf("%.3f: %s\n", m_timer.elapsedMilliseconds() / 1000.0, data); // Write elapsed time in seconds and data
        flightData.flush(); // Ensure data is written to the card
        return true;
    }
    
    // Write data from a device routine to its corresponding file
    bool writeData(DeviceRoutine* device) {
        char data[100];
        device->getData(data);

        File file = files[device];
        if (!file) {
            return false;
        }

        flightData.printf("%.3f: %s\n", m_timer.elapsedMilliseconds() / 1000.0, data); // Write elapsed time in seconds and data
        file.flush(); // Ensure data is written to the card
        return true;
    }

        
    void waitForSerialInput() {
        Serial.println("Waiting for serial input...");
        while (!Serial.available()) {
            delay(100);  // Wait for input, checking every 100 milliseconds
        }
        // Clear the buffer after receiving input
        while (Serial.available()) Serial.read();
    }

    // Destructor to close the file when the object is destroyed
    ~DataCard() {
        if (flightData) {
            flightData.close();
        }
    }


};