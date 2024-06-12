#ifndef ON_OFF_CONTROLLER_H
#define ON_OFF_CONTROLLER_H

#include "consts.h"


struct OnOffController {
    bool on = false;

    void turnOnOrOff(bool newOn) {
        on = newOn;
        digitalWrite(ON_OFF_PIN, on);
    }
};


OnOffController on_off_controller;


#endif
