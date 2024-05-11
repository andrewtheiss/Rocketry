#ifndef FLIGHTSTATUS_H
#define FLIGHTSTATUS_H

enum FlightStatus {

    IDLE,
    DATA_RECORDING,
    LAUNCHED,
    IN_FLIGHT,
    LANDED,
    ERROR,
    ERROR_NO_SD
};

#endif
