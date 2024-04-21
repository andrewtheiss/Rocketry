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
    protected:
        SPISettings m_settings;
        bool m_enabled = true;

};

#endif // DEVICEROUTINE_H
