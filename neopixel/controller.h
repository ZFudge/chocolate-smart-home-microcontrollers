#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Adafruit_NeoPixel.h>

#include "pixel.h"

namespace NeoPixel {
struct NeoPixelController : public Adafruit_NeoPixel {
    Adafruit_NeoPixel strip = Adafruit_NeoPixel();
    Pixel* pixels = NULL;

    byte numOfPixels;
    bool on = true;

    void init(byte dataPin, byte numOfPixels);
    void turnOnOff(const bool on) {
        this->on = on;
    };
};
}

#endif
