#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Adafruit_NeoPixel.h>

#include "consts.h"
#include "pixel.h"
#include "pir.h"


namespace NeoPixel {


struct NeoPixelController;

class PIRReader : public PIRSensor {
public:
    // pir needs to read/write the corresponding neo pixel controller's
    // ALL_PIXELS_BRIGHTNESS_ARE_CURRENT value inside motionDetected and
    // getCurrentSensorReading.
    NeoPixelController* neo_pixel_controller = NULL;
    bool motionDetected();
    bool getCurrentSensorReading();
    bool lastReading;
};


struct NeoPixelController : public Adafruit_NeoPixel {

Adafruit_NeoPixel strip = Adafruit_NeoPixel();
Pixel* pixels = NULL;

PIRReader* pir = NULL;

int numOfPixels = 0;
// Maximum number of pixel objects allowed.
int maxCount = 50;
bool on = true;
bool pirIsOn() {
    // Check any PIR sensor readings
    return pir != NULL && pir->motionDetected();
};
bool isOn() {
    return on || pirIsOn();
};
bool ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;
bool ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT = false;
bool twinkle = true;
bool transform = true;
byte brightness = 255;
byte ms = 0;

void init(const byte dataPin, const int numOfPixels, neoPixelType npType = NEO_GRB + NEO_KHZ800) {
    if (strip.numPixels() > 0 || numOfPixels == 0) return;

    // If the number of pixels is greater than the max count, the smaller of
    // the two values will be used for initializing the number of pixel objects,
    // with each pixel object representing neo pixels numbered as:
    //      n, n+maxCount, n+maxCount*2, ...
    // with the pattern ending when n+maxCount*index exceeds numOfPixels
    this->numOfPixels = min(numOfPixels, maxCount);

    strip.updateType(npType);
    strip.setPin(dataPin);
    strip.updateLength(numOfPixels);
    strip.begin();
    strip.show();

    pixels = new Pixel[numOfPixels];
    if (transform)
        for (int i = 0; i < this->numOfPixels; i++)
            pixels[i].setNewTransform();
}

void loop() {
    /* Main controller loop. Handles brightness twinkling, transforming RGB,
    turning on, turning off, and standing by. */

    if (!isOn() && this->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT && ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT)
        // Controller OFF and dimmed. Do nothing.
        return;

    if (!this->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT) {
        if (isOn()) {
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
                for (int i = 0; i < numOfPixels; i++) {
                    Pixel *pixel = &pixels[i];
                    if (pixel->brightness < this->brightness)
                        pixel->brightness++;
                    else if (pixel->brightness > this->brightness)
                        pixel->brightness--;
                    if (pixel->brightness != this->brightness)
                        DONE_CHANGING_BRIGHTNESS = false;
                    if (!this->transform)
                        // No twinkling or color transforms. Just apply brightness.
                        this->applyPixelSettingsToNeoPixel(i, pixel);
                }
                if (DONE_CHANGING_BRIGHTNESS)
                    ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = true;
            }
        } else {
            /* Dim from ON to OFF */
            bool DONE_TURNING_OFF = true;
            for (int i = 0; i < numOfPixels; i++) {
                Pixel *pixel = &pixels[i];
                if (pixel->brightness)
                    pixel->brightness--;
                if (pixel->brightness)
                    DONE_TURNING_OFF = false;
                if (!this->transform)
                    // Transform false. Twinkling won't apply when controller
                    // is off. Just apply brightness.
                    this->applyPixelSettingsToNeoPixel(i, pixel);
            }
            if (DONE_TURNING_OFF)
                ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = true;
        }
    }
    this->strip.show();

    if (!transform) {
        /* Either the controller is on but twinkle and transform are both off
            OR
            controller is dimming and twinkling doesn't apply. */
        if (!twinkle || (twinkle && !isOn())) {
            if (!ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT) {
                settleAnyTransforms();
                this->strip.show();
            }
            return;
        }
    }

    // Twinkle / Transform
    int ACTIVE_TRANSFORM_CYCLES_REMAINING = numOfPixels;
    for (int i = 0; i < this->numOfPixels; i++) {
        Pixel *pixel = &pixels[i];

        // Twinkle brightness
        if (twinkle && isOn())
            pixel->twinkle(this->brightness, this->transform);
        // Transform RGB
        if ((transform && isOn()) || pixel->transformStepsRemaining) {
            pixel->transform(this->isOn(), this->transform);
            if (pixel->transformStepsRemaining == 0)
                ACTIVE_TRANSFORM_CYCLES_REMAINING--;
        }

        // Brightness or RGB changed. Apply them.
        this->applyPixelSettingsToNeoPixel(i, pixel);
    }
    if (ACTIVE_TRANSFORM_CYCLES_REMAINING == 0)
        ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT = true;

    this->strip.show();

    delay(this->ms);
}

void turnOnOff(const bool on) {
    if (this->on == on)
        return;
    this->on = on;
    ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;
};

void setMS(const byte ms) {
    this->ms = ms;
};

void setMaxCount(const int maxCount) {
    if (maxCount == 0 || this->strip.numPixels() > 0) return;
    this->maxCount = maxCount;
};

void setBrightness(const byte brightness) {
    if (brightness == this->brightness)
        return;
    // If brightness setting would be dark enough to appear as though the
    // controller is turned off, just turn it off instead.
    if (brightness <= MIN_TWINKLE_BRIGHTNESS_THRESHOLD) {
        this->turnOnOff(false);
        return;
    }
    this->brightness = brightness;
    ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;
};

void setTwinkle(const bool twinkle) {
    this->twinkle = twinkle;

    if (twinkle) return;
    // Trigger brightness of pixels settling at controller brightness setting.
    for (int i = 0; i < numOfPixels; i++) {
        if (pixels[i].brightness != this->brightness) {
            ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;
            break;
        }
    }
};

void setTransform(const bool transform) {
    this->transform = transform;
    if (this->transform)
        ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT = false;
};

void settleAnyTransforms() {
    /* Increments any transform cycles towards completion, after transform has
    been turned off. */
    int ACTIVE_CYCLES_REMAINING = numOfPixels;
    for (int i = 0; i < numOfPixels; i++) {
        Pixel *pixel = &pixels[i];
        if (!pixel->transformStepsRemaining) {
            ACTIVE_CYCLES_REMAINING--;
            continue;
        }
        pixel->transform(this->isOn(), this->transform);
        this->applyPixelSettingsToNeoPixel(i, pixel);
    }
    if (ACTIVE_CYCLES_REMAINING == 0)
        ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT = true;
}

const byte applyBrightness(const float colorComponent, const double pixelBrightness) {
    /* Apply controller brightness and pixel brightness to color components. */
    double result = colorComponent;
    // Apply controller brightness.
    result *= (double)this->brightness / (double)255.0;
    // Apply pixel brightness.
    result *= pixelBrightness / (double)255.0;
    return round(result);
}

void applyPixelSettingsToNeoPixel(const int pixelIndex, const Pixel *pixel) {
    /* Apply pixel settings to corresponding NeoPixel. */
    const uint32_t pixelColor = strip.Color(
        applyBrightness(pixel->r, pixel->brightness),
        applyBrightness(pixel->g, pixel->brightness),
        applyBrightness(pixel->b, pixel->brightness)
    );

    // Apply pixel object color to the neo pixel of the corresponding index, as
    // well as any duplicate neo pixels that reuse the same pixel object.
    for (int i = pixelIndex; i < this->strip.numPixels(); i += this->maxCount)
        this->strip.setPixelColor(i, pixelColor);
};

void updateRGBs(String csvPalette) {
    /* Change color palette and set new target colors to the corresponding the
    color of the same color index used by each pixel when transform is on. */
    // Update palette.
    for (int i = 0; i < PIXELS_LENGTH; i++) {
        const byte colorComponent = csvPalette.substring(
            0,
            csvPalette.indexOf(',')
        ).toInt();
        csvPalette = csvPalette.substring(
            csvPalette.indexOf(',') + 1,
            csvPalette.length()
        );
        rgbs[i / 3][i % 3] = colorComponent;
    }
    // Update any transform color targets.
    for (int i = 0; i < numOfPixels; i++) {
        Pixel *pixel = &pixels[i];
        if (this->isOn()) {
            if (this->transform || pixel->transformStepsRemaining) {
                pixel->setNewTransformTargetFromCurrentState();
            } else if (!this->transform && !this->twinkle) {
                // Force smooth transition when controller is on but no
                // graceful way to
                pixel->transformStepsRemaining = 200;
                pixel->setNewTransformTargetFromCurrentState();
                ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT = false;
            }
        } else {
            pixel->setRGBFromIndex();
        }
    }
}

};


bool PIRReader::getCurrentSensorReading() {
    const bool motionDetected = getReading(pin);
    if (motionDetected) {
        lastMotionDetected = millis();
        neo_pixel_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;
    }
    return motionDetected;
};

bool PIRReader::motionDetected() {
    if (!armed) return false;

    const bool stillActive = isStillActive();
    const bool reading = getCurrentSensorReading();
    if (lastReading && !stillActive && !reading && neo_pixel_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT)
        neo_pixel_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

    lastReading = stillActive || reading;
    return lastReading;
}


void bindNPCToPIR(NeoPixelController *npc, PIRReader *pr) {
    /*Must be called in setup() function.
    Allow neo pixel controller and pir to reference each other. */
    npc->pir = pr;
    pr->neo_pixel_controller = npc;
}


}

#endif
