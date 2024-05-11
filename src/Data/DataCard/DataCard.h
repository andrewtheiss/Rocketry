#include <SD.h>
#include "../../Sensors/DeviceRoutine.h"
#include <map>
#include "../Timer.h"

const char filename[] = "flightData/flightLog00000.txt";

class DataCard {
    private:
        std::map<DeviceRoutine*, File> files;
        std::map<String, File> deviceLogs; // Map to store log files by device name
        File flightData;
        Timer m_timer;
        char fileNumberStr[6]; // Store the file number string
public:
    // Constructor
    DataCard() {
         strcpy(fileNumberStr, "00000");
    }

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
        char extension[] = ".txt";
        char fullFilename[32];  // Make sure this is large enough to hold the full filename

        if (!SD.begin(BUILTIN_SDCARD)) {
            Serial.println("The SD card failed to initialize.");
            while (1);
        }

        Serial.println("The SD card has been initialized.");

        // Find an unused filename
        for (unsigned int i = 0; i < 100000; i++) {  // Adjust the limit as needed
            sprintf(fileNumberStr, "%05u", i);  // Generate the next file number in the sequence
            snprintf(fullFilename, sizeof(fullFilename), "%s%s%s", baseFilename, fileNumberStr, extension);

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
    
    void initFiles(const std::vector<DeviceRoutine*>& devices) {
        for (DeviceRoutine* device : devices) {
            if (device->isLoggingEnabled()) {
                if (!initFile(device)) {
                    Serial.printf("Failed to initialize file for device: %s\n", device->getName());
                }
            }
        }
    }
    
    // Initialize a file for a specific device routine
    bool initFile(DeviceRoutine* device) {
        char baseFilename[32];
        char extension[] = ".txt";
        char fullFilename[50];  // Make sure this is large enough to hold the full filename

        snprintf(baseFilename, sizeof(baseFilename), "flightData/%s_", device->getName());
        snprintf(fullFilename, sizeof(fullFilename), "%s%s%s", baseFilename, fileNumberStr, extension);

        File file = SD.open(fullFilename, FILE_WRITE);
        if (!file) {
            Serial.println("Failed to open the log file.");
            return false;
        } else {
            Serial.printf("Log file %s opened for writing.\n", fullFilename);
            files[device] = file;
            deviceLogs[device->getName()] = file;
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

        File file = deviceLogs[device->getName()];
        if (!file) {
            return false;
        }

        file.printf("%.3f: %s\n", m_timer.elapsedMilliseconds() / 1000.0, data); // Write elapsed time in seconds and data
        file.flush(); // Ensure data is written to the card

        // Return the full file name
        char fullFilename[50];
        snprintf(fullFilename, sizeof(fullFilename), "flightData/%s_%s.txt", device->getName(), fileNumberStr);

        // Debug
        //Serial.printf("Data written to file: %s\n", fullFilename);
        
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
        for (auto& pair : deviceLogs) {
            if (pair.second) {
                pair.second.close();
            }
        }
    }


};