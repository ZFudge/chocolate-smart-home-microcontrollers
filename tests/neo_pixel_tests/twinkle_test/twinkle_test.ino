#line 2 "twinkle_test.ino"

#include <AUnitVerbose.h>

#include <chocolate-smart-home-microcontrollers.h>


#define TEST_DATA_PIN   6


using namespace NeoPixel;


NeoPixelController *test_controller;


class NeoPixelSinglePixel: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, 1);
        Pixel *pixel = &test_controller->pixels[0];
        pixel->targetBrightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};


testF(NeoPixelSinglePixel, test_setTwinkle_default) {
    assertTrue(test_controller->twinkle);
}


testF(NeoPixelSinglePixel, test_setTwinkle_false) {
    test_controller->setTwinkle(false);
    assertFalse(test_controller->twinkle);
}


testF(NeoPixelSinglePixel, test_twinkle_loop_brightness_up) {
    Pixel *pixel = &test_controller->pixels[0];

    assertEqual(pixel->brightness, 0);
    assertEqual(pixel->targetBrightness, 255);

    for (byte i = 0; i < 255; i++) {
        assertEqual(pixel->brightness, i);
        assertEqual(pixel->targetBrightness, 255);
        test_controller->loop();
    }

    // brightness and targetBrightness only compared at start of loop()
    assertEqual(pixel->brightness, 255);
    assertEqual(pixel->targetBrightness, 255);
    test_controller->loop();
    assertNotEqual(pixel->targetBrightness, 255);
}


class NeoPixelSinglePixelBright: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, 1);
        Pixel *pixel = &test_controller->pixels[0];
        pixel->brightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};


testF(NeoPixelSinglePixelBright, test_twinkle_loop_brightness_down) {
    Pixel *pixel = &test_controller->pixels[0];

    assertEqual(pixel->brightness, 255);
    assertEqual(pixel->targetBrightness, 0);

    for (byte i = 255; i > 0; i--) {
        assertEqual(pixel->brightness, i);
        assertEqual(pixel->targetBrightness, 0);
        test_controller->loop();
    }

    // brightness and targetBrightness only compared at start of loop()
    assertEqual(pixel->brightness, 0);
    assertEqual(pixel->targetBrightness, 0);
    test_controller->loop();
    assertNotEqual(pixel->targetBrightness, 0);
}


class NeoPixelSinglePixelMutable: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, 1);
        Pixel *pixel = &test_controller->pixels[0];
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

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};


testF(NeoPixelSinglePixelMutable, test_single_pixel_mutate_color) {
    Pixel *pixel = &test_controller->pixels[0];

    assertEqual(pixel->r, 0);
    assertEqual(pixel->g, 127);
    assertEqual(pixel->b, 255);
    assertEqual(pixel->colorIndex, 0);
    assertEqual(pixel->brightness, 0);
    assertEqual(pixel->targetBrightness, 0);

    test_controller->loop();

    assertNotEqual(pixel->colorIndex, 0);
    assertEqual(pixel->r, 77);
    assertEqual(pixel->g, 77);
    assertEqual(pixel->b, 77);
}


class NeoPixelTargetBrightnessBrighterThanMaxBrightness: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, 1);
        test_controller->brightness = 20;
        Pixel *pixel = &test_controller->pixels[0];
        pixel->targetBrightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};


testF(NeoPixelTargetBrightnessBrighterThanMaxBrightness, test_target_brightness_resets_when_brighter_than_max_brightness) {
    Pixel *pixel = &test_controller->pixels[0];

    assertEqual(test_controller->brightness, 20);
    assertEqual(pixel->targetBrightness, 255);

    test_controller->loop();

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
