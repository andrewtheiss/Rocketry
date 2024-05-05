#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <Arduino.h>
using namespace std::chrono;

typedef system_clock::time_point timePoint;               // save typing...

class Timer {
private:
    time_point<steady_clock> start_time; // Using steady_clock
    time_point<steady_clock> stop_time;

public:
    // Start the timer
    void start() {
        start_time = steady_clock::now();
    }

    // Stop the timer
    void stop() {
        stop_time = steady_clock::now();
    }

    // Returns the elapsed time in milliseconds
    uint32_t elapsedMilliseconds() {
        stop();
        auto duration = duration_cast<milliseconds>(stop_time - start_time);
        return static_cast<uint32_t>(duration.count());
    }

    // Returns the current time as a string
    String currentTime() {
        time_t now = system_clock::to_time_t(system_clock::now());
        return String(ctime(&now));
    }


    // Utility to print the elapsed time
    void printElapsedTime() {
        Serial.printf("Elapsed Time: %u ms\n", elapsedMilliseconds());
    }

    // Utility to print the current system time
    void printCurrentTime() {
        Serial.print("Current Time: ");
        Serial.println(currentTime());
    }
};

#endif