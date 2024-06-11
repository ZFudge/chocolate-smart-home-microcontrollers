#ifndef DUPLEX_MESSENGER_H_
#define DUPLEX_MESSENGER_H_

#include "Chocolate_Smart_Home_ESP_Connect.h"

#include "configuration.h"
#include "consts.h"
#include "on_off_controller.h"

String getControllerState() {
    /* Return the controller"s state in a format expected by the CSM server */
    return String(on_off_controller.onOff);
}

void processOnOffMsg(String message) {
    /* Process data from a message sent by the CSM server */
    const bool on = message.indexOf("&on=1") != -1;
    const bool off = message.indexOf("&on=0") != -1;

    if (on && off) return;
    if (!on && !off) return;
    if (on)
        on_off_controller.turnOnOrOff(true);
    else
        on_off_controller.turnOnOrOff(false);
}

#endif
