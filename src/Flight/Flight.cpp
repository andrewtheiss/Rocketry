#include "Flight.h"
#include "../Data/LEDs/StatusLED.h"

Flight::Flight()
    : currentStatus(IDLE), m_pStatusLED(nullptr), m_lsmSensor(nullptr), m_bmpSensor(nullptr), m_pTimer(nullptr),
      sampleIndex(0), averageElevation(0), averageAcceleration(0), lastSampleTime(0), launchDetectionStartTime(0) {}
      
void Flight::setStatus(FlightStatus status) {
    currentStatus = status;
    Serial.print("Updateing status to: " + status);
    if (m_pStatusLED) {
        m_pStatusLED->updateLEDs(); // Update LEDs whenever the status changes
    }
    if (m_pTimer) {
        Serial.print("Time of status change: ");
        Serial.println(m_pTimer->currentTime());
    }
}

void Flight::init(StatusLED* statusLED, const std::vector<DeviceRoutine*>& devices, Timer* timer) {
    m_pStatusLED = statusLED;
    m_devices = devices;
    m_pTimer = timer;
    setStatus(IDLE);
}

void Flight::init() {
    setStatus(ERROR);
}

void Flight::loop() {
    switch (currentStatus) {
        case DATA_RECORDING:
            LoopDataRecording();
            break;
        case IDLE_LAUNCH_READY:
            LoopIdleLaunchReady();
            break;
        case IN_FLIGHT_ASCENT:
            LoopAscent();
            break;
        case IN_FLIGHT_APOGEE:
        case IN_FLIGHT_MAIN_CHUTE_DEPLOYED:
            LoopAfterApogee();
            break;
        case LANDED:
            LoopFlightComplete();
            break;
        case ERROR:
        case ERROR_NO_SD:
            // Handle error states
            break;
        default:
            // Handle unexpected states
            break;
    }
}

void Flight::updateStatusIfOK(FlightStatus status) {
    if (currentStatus < ERROR) {
        setStatus(status);
    }
}

const char* Flight::getName() {
    return "Flight";
}

void Flight::getData(char data[]) {
    sprintf(data, "Flight Status: %d", currentStatus);
}