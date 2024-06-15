#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Adafruit_NeoPixel.h>

#include "pixel.h"

namespace Neopixel {
struct NeoPixelController : public Adafruit_NeoPixel {
    Adafruit_NeoPixel strip = Adafruit_NeoPixel();
    Pixel* pixels = NULL;

    byte numOfPixels;

    void init(byte dataPin, byte numOfPixels);
};
}

#endif
