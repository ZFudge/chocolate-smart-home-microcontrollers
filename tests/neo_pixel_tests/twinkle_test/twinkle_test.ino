#line 2 "twinkle_test.ino"

#include <AUnitVerbose.h>

#include <chocolate-smart-home-microcontrollers.h>


#define TEST_DATA_PIN   6


using namespace NeoPixel;


class ControllerSinglePixel: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, 1);
        test_controller.pixels[0].targetBrightness = 255;
    }
};

testF(ControllerSinglePixel, setTwinkle_default) {
    assertTrue(test_controller.twinkle);
}

testF(ControllerSinglePixel, setTwinkle_false) {
    test_controller.setTwinkle(false);
    assertFalse(test_controller.twinkle);
}

testF(ControllerSinglePixel, twinkle_loop_brightness_up) {
    Pixel *pixel = &test_controller.pixels[0];

    assertEqual(pixel->brightness, 0);
    assertEqual(pixel->targetBrightness, 255);

    for (byte i = 0; i < 255; i++) {
        assertEqual(pixel->brightness, i);
        assertEqual(pixel->targetBrightness, 255);
        test_controller.loop();
    }

    // brightness and targetBrightness only compared at start of loop()
    assertEqual(pixel->brightness, 255);
    assertEqual(pixel->targetBrightness, 255);
    test_controller.loop();
    assertNotEqual(pixel->targetBrightness, 255);
}


class ControllerSinglePixelBright: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    Pixel *pixel;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, 1);
        pixel = &test_controller.pixels[0];
        pixel->brightness = 255;
    }
};

testF(ControllerSinglePixelBright, twinkle_loop_brightness_down) {
    assertEqual(pixel->brightness, 255);
    assertEqual(pixel->targetBrightness, 0);

    for (byte i = 255; i > 0; i--) {
        assertEqual(pixel->brightness, i);
        assertEqual(pixel->targetBrightness, 0);
        test_controller.loop();
    }

    // brightness and targetBrightness only compared at start of loop()
    assertEqual(pixel->brightness, 0);
    assertEqual(pixel->targetBrightness, 0);
    test_controller.loop();
    assertNotEqual(pixel->targetBrightness, 0);
}


class ControllerSinglePixelMutableNoTransform: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    Pixel *pixel;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, 1);
        test_controller.transform = false;
        pixel = &test_controller.pixels[0];
        pixel->brightness = 0;
        pixel->targetBrightness = 0;
        rgbs[0][0] = 0;
        rgbs[0][1] = 127;
        rgbs[0][2] = 255;
        // Fill out remaining color components with all 77s.
        for (byte i = 1; i < 9; i++)
            for (byte ii = 0; ii < 3; ii++)
                rgbs[i][ii] = 77;
        pixel->colorIndex = 0;
        pixel->r = rgbs[0][0];
        pixel->g = rgbs[0][1];
        pixel->b = rgbs[0][2];
    }
};

testF(ControllerSinglePixelMutableNoTransform, single_pixel_mutate_color) {
    assertEqual(pixel->r, 0.0);
    assertEqual(pixel->g, 127.0);
    assertEqual(pixel->b, 255.0);
    assertEqual(pixel->colorIndex, 0);
    assertEqual(pixel->brightness, 0);
    assertEqual(pixel->targetBrightness, 0);

    test_controller.loop();

    assertNotEqual(pixel->colorIndex, 0);
    assertEqual(pixel->r, 77.0);
    assertEqual(pixel->g, 77.0);
    assertEqual(pixel->b, 77.0);
}


int getColorIndex_call_count = 0;
byte getColorIndex_1(byte colorIndex) {
    getColorIndex_call_count++;
    return 1;
};
int getTargetBrightnessInRange_call_count = 0;
byte getTargetBrightnessInRange_patched(byte m1, byte m2) {
    getTargetBrightnessInRange_call_count++;

    switch (getTargetBrightnessInRange_call_count) {
        case 1:
            return 200;
        case 2:
            return 0;
        default:
            return 100;
    }
};
class Controller_SinglePixelMutable: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    Pixel *pixel;
    void setup() override {
        aunit::TestOnce::setup();
        rgbs[0][0] = 0;
        rgbs[0][1] = 127;
        rgbs[0][2] = 255;
        rgbs[1][0] = 255;
        rgbs[1][1] = 0;
        rgbs[1][2] = 127;

        test_controller.setTransform(false);
        test_controller.init(TEST_DATA_PIN, 1);
        pixel = &test_controller.pixels[0];
        pixel->brightness = 0;
        pixel->targetBrightness = 0;
        pixel->colorIndex = 0;
        pixel->setRGBFromIndex();
        pixel->getColorIndexRandom = getColorIndex_1;
        pixel->getTargetBrightnessInRange = getTargetBrightnessInRange_patched;
        getColorIndex_call_count = 0;
        getTargetBrightnessInRange_call_count = 0;
    }

};

testF(Controller_SinglePixelMutable, color) {
    assertEqual(pixel->r, 0.0);
    assertEqual(pixel->g, 127.0);
    assertEqual(pixel->b, 255.0);
    assertEqual(pixel->colorIndex, 0);

    test_controller.loop();

    assertEqual(pixel->colorIndex, 1);
    assertEqual(pixel->r, 255.0);
    assertEqual(pixel->g, 0.0);
    assertEqual(pixel->b, 127.0);
}

testF(Controller_SinglePixelMutable, brightness) {
    assertEqual(pixel->brightness, 0);
    assertEqual(pixel->targetBrightness, 0);

    test_controller.loop();

    assertEqual(pixel->brightness, 1);
    assertEqual(pixel->targetBrightness, 200);
}

testF(Controller_SinglePixelMutable, call_counts) {
    /* Calls to methods used for controlling color and target brightness happen
    at the correct moment in the twinkle cycle. */
    assertEqual(getColorIndex_call_count, 0);
    assertEqual(getTargetBrightnessInRange_call_count, 0);

    // Initiate first, upward twinkle cycle with function calls.
    test_controller.loop();

    assertEqual(getColorIndex_call_count, 1);
    assertEqual(getTargetBrightnessInRange_call_count, 1);

    // Complete first, upward, twinkle cycle with no additional function calls.
    for (byte i = 1; i < 200; i++)
        test_controller.loop();
    assertEqual(getColorIndex_call_count, 1);
    assertEqual(getTargetBrightnessInRange_call_count, 1);

    // Initiate second, downward, twinkle cycle.
    test_controller.loop();

    // Color doesn't changes until a twinkle cycle ends beneath threshold.
    assertEqual(getColorIndex_call_count, 1);
    // targetBrightness still set at beginning of cycle, though.
    assertEqual(getTargetBrightnessInRange_call_count, 2);

    // Complete second, downward, twinkle cycle with no additional function calls.
    for (byte i = 199; i > 0; i--)
        test_controller.loop();
    assertEqual(getColorIndex_call_count, 1);
    assertEqual(getTargetBrightnessInRange_call_count, 2);

    // Initiate third, upward, twinkle cycle.
    test_controller.loop();
    assertEqual(getColorIndex_call_count, 2);
    assertEqual(getTargetBrightnessInRange_call_count, 3);
}


class NeoPixelTargetBrightnessBrighterThanMaxBrightness: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    Pixel *pixel;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, 1);
        test_controller.brightness = 20;
        pixel = &test_controller.pixels[0];
        pixel->targetBrightness = 255;
    }
};

testF(NeoPixelTargetBrightnessBrighterThanMaxBrightness, target_brightness_resets_when_brighter_than_max_brightness) {
    assertEqual(test_controller.brightness, 20);
    assertEqual(pixel->targetBrightness, 255);

    test_controller.loop();

    assertLess(pixel->targetBrightness, 20);
}


void setup() {
    Serial.begin(115200);
#if defined(EPOXY_DUINO)
    Serial.setLineModeUnix();
#endif
}


void loop() {
    aunit::TestRunner::run();
}
