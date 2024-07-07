#ifndef PIXEL_H
#define PIXEL_H

#include "rgbs.h"
#include "utils.h"


const byte MIN_TWINKLE_BRIGHTNESS_THRESHOLD = 5;


struct Pixel : public Utils::PixelUtils {

float r, g, b;
byte colorIndex;

byte brightness;

byte targetBrightness;

byte transformStepsRemaining;
double rTransformStep, gTransformStep, bTransformStep;


Pixel() {
    setColorRandomAny();
}

void setRGBFromIndex() {
    this->r = rgbs[colorIndex][0];
    this->g = rgbs[colorIndex][1];
    this->b = rgbs[colorIndex][2];
}

void setColorIndex(const byte index) {
    if (index > NUM_COLORS - 1)
        return;
    this->colorIndex = index;
}
void setColorRandom() {
    this->colorIndex = getColorIndexRandom(this->colorIndex);
    this->setRGBFromIndex();
}
void setColorRandomAny() {
    this->colorIndex = getColorRandomAny();
    this->setRGBFromIndex();
}

void setTargetBrightnessInRange(const byte minBrightness, const byte maxBrightness) {
    this->targetBrightness = getTargetBrightnessInRange(minBrightness, maxBrightness);
}
void setTargetBrightness(const byte targetBrightness) {
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
    this->transformStepsRemaining = getTransformStepsRemainingRandom();
}
void setNewTransform() {
    /* Start new transform cycle, within the same color palette */
    this->setTransformStepsRemainingRandom();
    // Re-anchor color components to palette to prevent floating point errors
    // from causing colors to drift.
    this->setRGBFromIndex();
    this->setNewTransformTargetFromCurrentState();
}
void setNewTransformTargetFromCurrentState() {
    const double r1 = this->r;
    const double g1 = this->g;
    const double b1 = this->b;
    this->setColorRandom();
    const double r2 = this->r;
    const double g2 = this->g;
    const double b2 = this->b;
    const double rDiff = r2 - r1;
    const double gDiff = g2 - g1;
    const double bDiff = b2 - b1;

    this->rTransformStep = rDiff / (double)transformStepsRemaining;
    this->gTransformStep = gDiff / (double)transformStepsRemaining;
    this->bTransformStep = bDiff / (double)transformStepsRemaining;
    this->r = r1;
    this->g = g1;
    this->b = b1;
}

void transform(const bool controllerTransform) {
    if (transformStepsRemaining <= 0)
        setNewTransform();

    this->r += rTransformStep;
    this->g += gTransformStep;
    this->b += bTransformStep;
    this->transformStepsRemaining--;

    // Unset steps when transform is off and transform cycle has finished.
    if (!controllerTransform && transformStepsRemaining == 0) {
        this->rTransformStep = 0.0;
        this->gTransformStep = 0.0;
        this->bTransformStep = 0.0;
    }
}


};

#endif
