#ifndef PIXEL_H
#define PIXEL_H

#include "rgbs.h"
#include "utils.h"




struct Pixel {

float r, g, b;
byte colorIndex;

byte brightness;

byte targetBrightness;

byte transformStepsRemaining;
float rTransformStep, gTransformStep, bTransformStep;

const byte MIN_TWINKLE_BRIGHTNESS_THRESHOLD = 5;

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

void setTargetBrightnessInRange(byte minBrightness, byte maxBrightness) {
    this->targetBrightness = random(minBrightness, maxBrightness);
}
void setTargetBrightness(byte targetBrightness) {
    this->targetBrightness = targetBrightness;
}

void incrementBrightness() {
    this->brightness += brightness < targetBrightness ? 1 : -1;
}

void twinkle(const byte maxBrightness, const bool transform) {
    if (targetBrightness > maxBrightness) {
        setTargetBrightnessInRange(0, maxBrightness);
    }
    if (brightness == targetBrightness) {
        while (brightness == targetBrightness)
            setTargetBrightnessInRange(0, maxBrightness);
        if (brightness <= MIN_TWINKLE_BRIGHTNESS_THRESHOLD && !transform)
            setColorRandom();
    }
    incrementBrightness();
}

void setTransformStepsRemainingRandom() {
    transformStepsRemaining = random(170, 255);
}
void setNewTransform() {
    this->setTransformStepsRemainingRandom();
    const byte r1 = this->r;
    const byte g1 = this->g;
    const byte b1 = this->b;
    this->setColorRandom();
    const byte r2 = this->r;
    const byte g2 = this->g;
    const byte b2 = this->b;
    const float rDiff = (float)r1 - r2;
    const float gDiff = (float)g1 - g2;
    const float bDiff = (float)b1 - b2;
    Serial.println(String(rDiff)+","+String(gDiff)+","+String(bDiff));
    this->rTransformStep = rDiff / (float)transformStepsRemaining;
    this->gTransformStep = gDiff / (float)transformStepsRemaining;
    this->bTransformStep = bDiff / (float)transformStepsRemaining;
    Serial.println();
}

void transform() {
    // if (transformStepsRemaining <= 0)
    //     setNewTransform();
    // this->r += rTransformStep;
    // this->g += gTransformStep;
    // this->b += gTransformStep;
    // this->transformStepsRemaining--;
}


};

#endif
