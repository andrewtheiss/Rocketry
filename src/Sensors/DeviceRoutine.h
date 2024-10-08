// DeviceRoutine.h
#ifndef DEVICEROUTINE_H
#define DEVICEROUTINE_H

#include <SPI.h>

// Interface for microelectronics program routines.
class DeviceRoutine {
public:
    virtual ~DeviceRoutine() {}

    // Initialize the device. This method should setup the device's initial configuration.
    virtual void init() = 0;

    // Loop method to be executed repeatedly. This should contain the main functionality
    // of the device that needs to be executed continuously.
    virtual void loop() = 0;

    // Retrieve data in a standardized format.
    virtual void getData(char data[]) = 0;
    
    // Get the name of the device
    virtual const char* getName() = 0;

    // Enable the device routine.
    void enable() {
        m_enabled = true;
    }

    // Disable the device routine.
    void disable() {
        m_enabled = false;
    }

    // Check if the device routine is enabled.
    bool isEnabled() {
        return m_enabled;
    }

    // Check if logging is enabled.
    bool isLoggingEnabled() const {
        return m_loggingEnabled;
    }

    protected:
        SPISettings m_settings;
        bool m_enabled = true;
        bool m_loggingEnabled = false; // Logging flag

};

#endif // DEVICEROUTINE_H
