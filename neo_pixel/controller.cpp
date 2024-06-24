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
    /* Main controller loop. Handles brightness twinkling, transforming RGB,
       turning on, turning off, and standing by. */

    if (!on && ALL_PIXELS_BRIGHTNESS_ARE_CURRENT)
        // Controller OFF and dimmed. Do nothing.
        return;

    if (!ALL_PIXELS_BRIGHTNESS_ARE_CURRENT) {
        if (on) {
            /* Brighten from   OFF to ON
               Dim from        ON to OFF
               Brighten from   lower brightness to higher brightness (when twinkle is off)
               Dim from        higher brightness to lower brightness (when twinkle is off) */
            if (twinkle) {
                // Pixels will automatically twinkle at the appropriate brightness.
                ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = true;
            } else {
                // Manually turn on.
                bool DONE_CHANGING_BRIGHTNESS = true;
                for (byte i = 0; i < numOfPixels; i++) {
                    Pixel *pixel = &pixels[i];
                    if (pixel->brightness < this->brightness) {
                        pixel->brightness++;
                    } else if (pixel->brightness > this->brightness) {
                        pixel->brightness--;
                    }
                    if (pixel->brightness != this->brightness)
                        DONE_CHANGING_BRIGHTNESS = false;
                    if (!this->transform)
                        // No twinkling or color transforms. Just apply brightness.
                        this->applyBrightnessAndOrRGBtoNeoPixel(i, pixel);
                }
                if (DONE_CHANGING_BRIGHTNESS)
                    this->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = true;
            }
        } else {
            /* Dim from ON to OFF */
            bool DONE_TURNING_OFF = true;
            for (byte i = 0; i < numOfPixels; i++) {
                Pixel *pixel = &pixels[i];
                if (pixel->brightness)
                    pixel->brightness--;
                if (pixel->brightness)
                    DONE_TURNING_OFF = false;
                if (!this->transform)
                    // Transform false. Twinkling won't apply when controller is off. Just apply brightness. """
                    this->applyBrightnessAndOrRGBtoNeoPixel(i, pixel);
            }
            if (DONE_TURNING_OFF)
                this->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = true;
        }
    }

    if (!transform)
        /* Either the controller is on but twinkle and transform are both off
            OR
            controller is dimming and twinkling doesn't apply. */
        if (!twinkle or (twinkle and !on))
            return;

    // Twinkle / Transform
    for (byte i = 0; i < numOfPixels; i++) {
        Pixel *pixel = &pixels[i];

        // Twinkle brightness
        if (twinkle && on)
            pixel->twinkle(this->brightness);
        // Transform RGB
        if (transform)
            pixel->transform();

        // Brightness or RGB changed. Apply them.
        this->applyBrightnessAndOrRGBtoNeoPixel(i, pixel);
    }

    this->strip.show();
}

