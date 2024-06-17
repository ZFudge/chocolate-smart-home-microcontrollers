#include "controller.h"


void NeoPixel::NeoPixelController::init(byte dataPin, byte numOfPixels) {
    strip.setPin(dataPin);
    strip.updateLength(numOfPixels);
    strip.updateType(NEO_GRB + NEO_KHZ800);
    strip.begin();
    strip.show();

    this->numOfPixels = numOfPixels;
    pixels = new Pixel[numOfPixels];
}
