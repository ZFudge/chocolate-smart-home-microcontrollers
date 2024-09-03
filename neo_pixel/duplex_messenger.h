#ifndef DUPLEX_MESSENGER_H
#define DUPLEX_MESSENGER_H

#include "controller.h"


bool allTwinkleColorsAreCurrent(NeoPixel::NeoPixelController *controller) {
    /*Returns false if controller is only in twinkle mode and not all pixels'
    rgb values match their corresponding colorIndex color; true otherwise.*/
    if (!controller->twinkle || controller->transform)
        return true;

    for (int i = 0; i < controller->numOfPixels; i++) {
        Pixel *pixel = &controller->pixels[i];

        if ((byte)pixel->r != rgbs[pixel->colorIndex][0] ||
            (byte)pixel->g != rgbs[pixel->colorIndex][1] ||
            (byte)pixel->b != rgbs[pixel->colorIndex][2])
            return false;
    }
    return true;
}


namespace DuplexMessenger {


String getNeoPixelControllerState(NeoPixel::NeoPixelController *controller) {
    /* Return the controller's state in a format expected by the CSM server */
    const byte boolsByte = byte(controller->on) |
                           byte(controller->twinkle)            << 1 |
                           byte(controller->transform)          << 2 |
                           byte(allTwinkleColorsAreCurrent(controller)) << 3;

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

    // PIR start
    if (Utils::hasParam("pir_armed", message)) {
        if (controller->pir == NULL) return;
        const bool arm = Utils::getByteValueUsingKey(message, "pir_armed");
        controller->pir->arm(arm);
    }

    if (Utils::hasParam("pir_timeout", message)) {
        if (controller->pir == NULL) return;
        const byte timeout = Utils::getByteValueUsingKey(message, "pir_timeout");
        controller->pir->setTimeoutInSeconds(timeout);
    }
    // PIR end

}


};


#endif
