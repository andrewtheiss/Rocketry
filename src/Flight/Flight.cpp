#include "Flight.h"
#include "../Data/LEDs/StatusLED.h"

void Flight::setStatus(FlightStatus status) {
    currentStatus = status;
    Serial.print("Updateing status to: " + status);
    if (m_pStatusLED) {
        m_pStatusLED->updateLEDs(); // Update LEDs whenever the status changes
    }
}

void Flight::init(StatusLED* statusLED, const std::vector<DeviceRoutine*>& devices) {
    m_pStatusLED = statusLED;
    m_devices = devices;
    setStatus(IDLE);
}

void Flight::init() {
    setStatus(ERROR);
}

void Flight::loop() {
    // If accel is greater than x amount for a certain time, change status to in flight
    // Placeholder for future flight logic
    // Implement flight logic here
    // You can now use m_lsmSensor and m_bmpSensor to access sensor data
    // For example:
    if (m_lsmSensor && m_bmpSensor) {
        // Read acceleration data
        float accelX, accelY, accelZ;
        //m_lsmSensor->getAcceleration(accelX, accelY, accelZ);

        // Read altitude data
        //float altitude = m_bmpSensor->readAltitude();

        // Use this data to make decisions about the flight status
        // ...
    }
    /*
    4161.522: Accel: M: 2.229197  1.99951, -0.86389, -0.47430
4161.571: Accel: M: 2.107912  1.99951, -0.66546, -0.04901
4161.620: Accel: M: 2.160149  1.99951, -0.80731, 0.12823
4161.669: Accel: M: 2.270066  1.99951, -1.06176, 0.16674
4161.719: Accel: M: 2.274204  1.99951, -1.07464, 0.13818
4161.768: Accel: M: 2.268551  1.99951, -1.04718, -0.22735
4161.824: Accel: M: 2.399820  1.99951, -1.23449, -0.48693
4161.873: Accel: M: 2.398549  1.99951, -1.31030, -0.19519
4161.922: Accel: M: 2.553783  1.99951, -1.58764, 0.05603
4161.972: Accel: M: 2.546149  1.99951, -1.42156, 0.68115
4162.021: Accel: M: 2.605616  1.99951, -1.64910, 0.26763
4162.070: Accel: M: 2.449767  1.99951, -1.32720, -0.49176
4162.119: Accel: M: 2.756639  1.99951, -1.89245, 0.14007
4162.168: Accel: M: 2.717427  1.99951, -1.81127, 0.32501
4162.218: Accel: M: 2.561098  1.99951, -1.56542, -0.33258
4162.267: Accel: M: 2.727664  1.99951, -1.85150, 0.11853
4162.316: Accel: M: 2.757681  1.99951, -1.89898, 0.02471
 ** Check when the accel is greater than 2.5 for a certain amount of time.

 Create a log for flight data
    */
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