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
    bool twinkle = true;
    byte brightness = 255;

    void init(byte dataPin, byte numOfPixels);
    void turnOnOff(const bool on) {
        this->on = on;
    };
    void setBrightness(const byte brightness) {
        this->brightness = brightness;
    };
    void setTwinkle(const bool twinkle) {
        this->twinkle = twinkle;
    };
};
}

#endif
