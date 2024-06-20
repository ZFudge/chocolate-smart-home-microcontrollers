#ifndef UTILS_H
#define UTILS_H

namespace Utils {


byte getRandomIndex(byte oldIndex) {
    /* Return a different, random index */
    byte i = oldIndex;
    while (i == oldIndex)
        i = random(0, NUM_COLORS);
    return i;
}

}

#endif
