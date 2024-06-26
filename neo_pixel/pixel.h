#ifndef PIXEL_H
#define PIXEL_H

#include "rgbs.h"
#include "utils.h"

const int MIN_TWINKLE_BRIGHTNESS_THRESHOLD = 5;


struct Pixel {
    byte r, g, b;
    byte colorIndex;

    byte targetBrightness;
    byte brightness;

    Pixel() {
        setColorRandomAny();
    }

    void setRGBFromIndex() {
        this->r = rgbs[colorIndex][0];
        this->g = rgbs[colorIndex][1];
        this->b = rgbs[colorIndex][2];
    }

    void setColorIndex(byte index) {
        this->colorIndex = index;
    }
    void setColorRandom() {
        this->colorIndex = Utils::getRandomIndex(this->colorIndex);
        this->setRGBFromIndex();
    }
    void setColorRandomAny() {
        this->colorIndex = Utils::getRandomIndex(NUM_COLORS + 1);
        this->setRGBFromIndex();
    }


    void twinkle(byte maxBrightness) {
        if (targetBrightness > maxBrightness) {
            setTargetBrightnessInRange(0, maxBrightness);
        }
        if (brightness == targetBrightness) {
            while (brightness == targetBrightness)
                setTargetBrightnessInRange(0, maxBrightness);
            if (brightness <= MIN_TWINKLE_BRIGHTNESS_THRESHOLD)
                setColorRandom();
        }
        incrementBrightness();
    }


    void transform() {
        // TODO
    }


    void setTargetBrightnessInRange(byte minBrightness, byte maxBrightness) {
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
