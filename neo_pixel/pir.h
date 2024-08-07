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

byte timeoutInSeconds = 60;
long lastMotionDetected = 0;

bool (*getReading)(const byte) = getReadingFromPin;
bool getCurrentSensorReading() {
    const bool motionDetected = getReading(pin);
    if (motionDetected)
        lastMotionDetected = millis();
    return motionDetected;
};

bool isStillActive() {
    /* Returns true if motion has been detected within the time window defined
    by timeoutInSeconds */
    const int secondsLapsedSinceLastReading = (millis() - lastMotionDetected) / 1000;
    return secondsLapsedSinceLastReading < timeoutInSeconds;
};

PIRSensor(const byte pin) {
    this->pin = pin;
    pinMode(pin, INPUT);
}

void setTimeoutInSeconds(const byte timeout) {
    this->timeoutInSeconds = timeout;
}

bool motionDetected() {
    return (
        this->getCurrentSensorReading() ||
        this->isStillActive()
    );
}

};

#endif
