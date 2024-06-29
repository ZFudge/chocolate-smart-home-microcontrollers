#ifndef UTILS_H
#define UTILS_H

#include "rgbs.h"

namespace Utils {


byte getRandomIndex(byte oldIndex) {
    /* Return a different, random index */
    byte i = oldIndex;
    while (i == oldIndex)
        i = random(0, NUM_COLORS);
    return i;
}

byte getTargetBrightnessInRangeDefault(byte minBrightness, byte maxBrightness) {
    return random(minBrightness, maxBrightness);
}

byte getTransformStepsRemainingRandomDefault() {
    return random(170, 255);
}

byte getColorIndexRandomDefault(byte colorIndex) {
    return getRandomIndex(colorIndex);
};

byte getColorRandomAnyDefault() {
    return getRandomIndex(NUM_COLORS + 1);
};

struct PixelUtils {
/* Util methods using randomization. Overwritable in testing. */
    byte (*getColorIndexRandom)(byte colorIndex) = getColorIndexRandomDefault;
    byte (*getColorRandomAny)() = getColorRandomAnyDefault;
    byte (*getTargetBrightnessInRange)(byte minBrightness, byte maxBrightness)
        = getTargetBrightnessInRangeDefault;
    byte (*getTransformStepsRemainingRandom)()
        = getTransformStepsRemainingRandomDefault;
};


}

#endif
