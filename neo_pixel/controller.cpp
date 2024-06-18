#include "controller.h"


void NeoPixel::NeoPixelController::init(const byte dataPin, const byte numOfPixels) {
    strip.setPin(dataPin);
    strip.updateLength(numOfPixels);
    strip.updateType(NEO_GRB + NEO_KHZ800);
    strip.begin();
    strip.show();

    this->numOfPixels = numOfPixels;
    pixels = new Pixel[numOfPixels];
}


void NeoPixel::NeoPixelController::loop() {
    for (byte i = 0; i < numOfPixels; i++) {
        Pixel *pixel = &pixels[i];

        if (twinkle) {
            while (pixel->brightness == pixel->targetBrightness)
                pixel->setTargetBrightness(10, brightness);
            pixel->incrementBrightness();
        }
    }
}
