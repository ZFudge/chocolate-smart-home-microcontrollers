#include "pixel.h"

#include "consts.cpp"


void Pixel::setRGBFromIndex() {
    this->r = rgbs[colorIndex][0];
    this->g = rgbs[colorIndex][1];
    this->b = rgbs[colorIndex][2];
}

void Pixel::setColorIndex(byte index) {
    this->colorIndex = index;
}
void Pixel::setColorRandom() {
    this->colorIndex = Utils::getRandomIndex(this->colorIndex);
    this->setRGBFromIndex();
}
void Pixel::setColorRandomAny() {
    this->colorIndex = Utils::getRandomIndex(NUM_COLORS + 1);
    this->setRGBFromIndex();
}


void Pixel::twinkle(byte maxBrightness) {
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


void Pixel::transform() {
    // TODO
}
