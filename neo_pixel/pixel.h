#ifndef PIXEL_H
#define PIXEL_H

#include "rgbs.h"


struct Pixel {
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
};

#endif
