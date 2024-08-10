#ifndef DUPLEX_MESSENGER_H
#define DUPLEX_MESSENGER_H

#include "controller.h"

namespace DuplexMessenger {


String getNeoPixelControllerState(NeoPixel::NeoPixelController *controller) {
    /* Return the controller's state in a format expected by the CSM server */
    const byte boolsByte = byte(controller->on) |
                           byte(controller->twinkle)   << 1 |
                           byte(controller->transform) << 2;

    return String(boolsByte) + "," +
           String(controller->ms) + "," +
           String(controller->brightness) + "," +
           getCsvRGBs();
}


void processNeoPixelMsg(String message, NeoPixel::NeoPixelController *controller) {
    if (Utils::hasParam("on", message)) {
        const bool on = (bool)Utils::getByteValueUsingKey(message, "on");
        controller->turnOnOff(on);
    }

    if (Utils::hasParam("ms", message)) {
        const byte ms = Utils::getByteValueUsingKey(message, "ms");
        controller->setMS(ms);
    }

    if (Utils::hasParam("brightness", message)) {
        const byte brightness = Utils::getByteValueUsingKey(message, "brightness");
        controller->setBrightness(brightness);
    }

    if (Utils::hasParam("twinkle", message)) {
        const bool twinkle = Utils::getByteValueUsingKey(message, "twinkle");
        controller->setTwinkle(twinkle);
    }

    if (Utils::hasParam("transform", message)) {
        const bool transform = Utils::getByteValueUsingKey(message, "transform");
        controller->setTransform(transform);
    }

    if (Utils::hasParam("palette", message)) {
        const String keyStart = "palette=";
        // Trim up to the param key.
        const String msg = message.substring(
            message.indexOf(keyStart) + keyStart.length(),
            message.length()
        );
        const String paletteCsvString = msg.substring(0, msg.indexOf(";"));
        controller->updateRGBs(paletteCsvString);
    }

    if (Utils::hasParam("armed", message)) {
        const bool arm = Utils::getByteValueUsingKey(message, "armed");
        controller->armPir(arm);
    }
}


};


#endif
