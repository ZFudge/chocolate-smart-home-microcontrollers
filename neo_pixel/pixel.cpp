#include "pixel.h"


void Pixel::setColorRandom() {
    this->colorIndex = Utils::getRandomIndex(NUM_COLORS + 1);
    this->r = rgbs[colorIndex][0];
    this->g = rgbs[colorIndex][1];
    this->b = rgbs[colorIndex][2];
}

void Pixel::twinkle(byte maxBrightness) {
    if (brightness == targetBrightness) {
        while (brightness == targetBrightness)
            setTargetBrightness(10, maxBrightness);
        setColorRandom();
    }
    incrementBrightness();
}
