#include <SD.h>
const char filename[] = "flightData/flightLog00000.txt";

class DataCard {
public:
    // Constructor
    DataCard() {
        
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

    void init() {
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
    
    // Write data to the specified file on the SD card
    bool writeData(const char* data) {
        if (!flightData) {
            // Re-open the file if it's not open
            flightData = SD.open(filename, FILE_WRITE);
            if (!flightData) return false;
        }

        flightData.println(data); // Write data and a newline
        flightData.flush(); // Ensure data is written to the card
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

    private:
        File flightData;

};