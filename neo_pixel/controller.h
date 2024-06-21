#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Adafruit_NeoPixel.h>

#include "pixel.cpp"

namespace NeoPixel {


struct NeoPixelController : public Adafruit_NeoPixel {
    Adafruit_NeoPixel strip = Adafruit_NeoPixel();
    Pixel* pixels = NULL;

    byte numOfPixels;
    bool on = true;
    bool ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = true;
    bool twinkle = true;
    bool transform = true;
    byte brightness = 255;

    void init(byte dataPin, byte numOfPixels);
    void loop();

    void turnOnOff(const bool on) {
        if (this->on == on)
            return;
        this->on = on;
        this->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;
    };
    void setBrightness(const byte brightness) {
        if (brightness == this->brightness)
            return;
        this->brightness = brightness;
        this->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;
    };
    void setTwinkle(const bool twinkle) {
        this->twinkle = twinkle;
    };
    void setTransform(const bool transform) {
        this->transform = transform;
    };
};

}

#endif
