#ifndef PIXEL_H
#define PIXEL_H

#include "rgbs.h"
#include "utils.h"


struct Pixel {
    byte r, g, b;
    byte colorIndex;

    byte targetBrightness;
    byte brightness;

    void setTargetBrightness(byte minBrightness, byte maxBrightness) {
        this->targetBrightness = random(minBrightness, maxBrightness);
    }
    void setTargetBrightness(byte targetBrightness) {
        this->targetBrightness = targetBrightness;
    }
    void incrementBrightness() {
        this->brightness += brightness < targetBrightness ? 1 : -1;
    }
    void twinkle(byte maxBrightness);
    void setColorRandom();
    Pixel() {
        setColorRandom();
    }
};

#endif
