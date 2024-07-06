#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Adafruit_NeoPixel.h>

#include "consts.h"
#include "pixel.h"

namespace NeoPixel {


struct NeoPixelController : public Adafruit_NeoPixel {

Adafruit_NeoPixel strip = Adafruit_NeoPixel();
Pixel* pixels = NULL;

byte numOfPixels;
bool on = true;
bool ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = true;
bool twinkle = true;
bool transform = true;
byte brightness = 255;

void init(const byte dataPin, const byte numOfPixels) {
    if (strip.numPixels()) return;

    strip.setPin(dataPin);
    strip.updateLength(numOfPixels);
    strip.updateType(NEO_GRB + NEO_KHZ800);
    strip.begin();
    strip.show();

    this->numOfPixels = numOfPixels;
    pixels = new Pixel[numOfPixels];
    if (transform)
        for (byte i = 0; i < numOfPixels; i++)
            pixels[i].setNewTransform();
}

void loop() {
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
                    if (!this->transform) {
                        if (pixel->transformStepsRemaining)
                            pixel->transform(this->transform);
                        // No twinkling or color transforms. Just apply brightness.
                        this->applyPixelSettingsToNeoPixel(i, pixel);
                    }
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
                if (!this->transform) {
                    if (pixel->transformStepsRemaining)
                        pixel->transform(this->transform);
                    // Transform false. Twinkling won't apply when controller is off. Just apply brightness. """
                    this->applyPixelSettingsToNeoPixel(i, pixel);
                }
            }
            if (DONE_TURNING_OFF)
                this->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = true;
        }
    }

    if (!transform)
        /* Either the controller is on but twinkle and transform are both off
            OR
            controller is dimming and twinkling doesn't apply. */
        if (!twinkle || (twinkle && !on))
            return settleAnyTransforms();

    // Twinkle / Transform
    for (byte i = 0; i < numOfPixels; i++) {
        Pixel *pixel = &pixels[i];

        // Twinkle brightness
        if (twinkle && on)
            pixel->twinkle(this->brightness, this->transform);
        // Transform RGB
        if (transform || pixel->transformStepsRemaining)
            pixel->transform(this->transform);

        // Brightness or RGB changed. Apply them.
        this->applyPixelSettingsToNeoPixel(i, pixel);
    }

    this->strip.show();
}

void turnOnOff(const bool on) {
    if (this->on == on)
        return;
    this->on = on;
    this->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;
};
void setBrightness(const byte brightness) {
    if (brightness == this->brightness)
        return;
    this->brightness = brightness;
    this->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;
};
void setTwinkle(const bool twinkle) {
    this->twinkle = twinkle;
};
void setTransform(const bool transform) {
    this->transform = transform;
};
void settleAnyTransforms() {
    /* Increment any transform cycles towards completion when transform is turned off. */
    for (byte i = 0; i < numOfPixels; i++) {
        Pixel *pixel = &pixels[i];
        if (!pixel->transformStepsRemaining)
            continue;
        pixel->transform(this->transform);
        this->applyPixelSettingsToNeoPixel(i, pixel);
    }
}

const byte applyBrightness(const float colorComponent, const float pixelBrightness) {
    /* Apply controller brightness and pixel brightness to color components. */
    double result = colorComponent;
    result *= (double)this->brightness / (double)255.0;
    result *= pixelBrightness / (double)255.0;
    return round(result);
}

void applyPixelSettingsToNeoPixel(const byte pixelIndex, const Pixel *pixel) {
    /* Apply pixel settings to corresponding NeoPixel. */
    const uint32_t pixelColor = strip.Color(
        applyBrightness(pixel->r, pixel->brightness),
        applyBrightness(pixel->g, pixel->brightness),
        applyBrightness(pixel->b, pixel->brightness)
    );
    this->strip.setPixelColor(pixelIndex, pixelColor);
};

void updateRGBS(String csvPalette) {
    /* Change color palette and set new target colors to the corresponding the
    color of the same color index used by each pixel when transform is on. */
    // Update palette.
    for (byte i = 0; i < PIXELS_LENGTH; i++) {
        const byte colorComponent = byte(csvPalette.substring(0, csvPalette.indexOf(',')).toInt());
        csvPalette = csvPalette.substring(csvPalette.indexOf(',') + 1, csvPalette.length());
        rgbs[i / 3][i % 3] = colorComponent;
    }
    // Update any transform color targets.
    for (byte i = 0; i < numOfPixels; i++) {
        Pixel *pixel = &pixels[i];
        if (!this->transform && !pixel->transformStepsRemaining)
            continue;
        pixel->setNewTransformTargetFromCurrentState();
    }
}

};

}

#endif
