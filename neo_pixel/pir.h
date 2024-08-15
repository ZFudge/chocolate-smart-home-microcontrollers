#ifndef PIRSENSOR_H_
#define PIRSENSOR_H_

#include <Arduino.h>


bool getReadingFromPin(const byte pin) {
    /* Defined outside of PIRSensor so PIRSensor.getReading can be mocked
    during testing. */
    return digitalRead(pin);
}

class PIRSensor {
public:
byte pin;

bool armed = true;
void arm(const bool armed) {
    this->armed = armed;
}

byte timeoutInSeconds = 60;
long lastMotionDetected = 0;

bool (*getReading)(const byte) = getReadingFromPin;

PIRSensor(const byte pin) {
    this->pin = pin;
    pinMode(pin, INPUT);
}

void setTimeoutInSeconds(const byte timeout) {
    this->timeoutInSeconds = timeout;
}

};

#endif
