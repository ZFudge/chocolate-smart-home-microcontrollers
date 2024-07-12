#ifndef UTILS_H
#define UTILS_H

#include "rgbs.h"

namespace Utils {


byte getRandomIndex(const byte oldIndex) {
    /* Return a different, random index */
    byte i = oldIndex;
    while (i == oldIndex)
        i = random(0, NUM_COLORS);
    return i;
}

bool hasParam(const String param, const String msg) {
    return msg.indexOf(param + (String)"=") != -1;
}

byte getByteValueUsingKey(String msg, String key) {
    const String keyStart = key + (String)"=";
    // Trim up to the param value.
    msg = msg.substring(
        msg.indexOf(keyStart) + keyStart.length(),
        msg.length()
    );
    const byte value = msg.substring(0, msg.indexOf(";")).toInt();
    return value;
}


byte getColorRandomAnyDefault() {
    return getRandomIndex(NUM_COLORS + 1);
};

byte getTransformStepsRemainingRandomDefault() {
    return random(170, 255);
}

byte getColorIndexRandomDefault(const byte colorIndex) {
    return getRandomIndex(colorIndex);
};

byte getTargetBrightnessInRangeDefault(const byte minBrightness, const byte maxBrightness) {
    return random(minBrightness, maxBrightness);
}

struct PixelUtils {
/* Util methods using randomization. Overwritable in testing. */
    byte (*getColorRandomAny)() = getColorRandomAnyDefault;
    byte (*getTransformStepsRemainingRandom)() = getTransformStepsRemainingRandomDefault;
    byte (*getColorIndexRandom)(const byte) = getColorIndexRandomDefault;
    byte (*getTargetBrightnessInRange)(const byte, const byte) = getTargetBrightnessInRangeDefault;
};

}

#endif
