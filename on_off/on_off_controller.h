#ifndef ON_OFF_CONTROLLER_H_
#define ON_OFF_CONTROLLER_H_

#include "consts.h"


struct OnOffController {
    bool onOff = false;

    void turnOnOrOff(bool newOnOff) {
        Serial.print(newOnOff ? "ON" : "OFF");
        if (newOnOff == onOff) return;
        onOff = newOnOff;
        digitalWrite(ON_OFF_PIN, onOff);
    }
};


OnOffController on_off_controller;


#endif
