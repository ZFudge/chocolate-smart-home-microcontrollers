#line 2 "twinkle_test.ino"

#include <AUnitVerbose.h>
#include <chocolate-smart-home-microcontrollers.h>

#define TEST_DATA_PIN   6


using namespace NeoPixel;


NeoPixelController *test_controller;


class NeopixelSinglePixel: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, 1);
        Pixel *pixel = &test_controller->pixels[0];
        pixel->setTargetBrightness(255);
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};


testF(NeopixelSinglePixel, test_setTwinkle_default) {
    assertTrue(test_controller->twinkle);
}


testF(NeopixelSinglePixel, test_setTwinkle_false) {
    test_controller->setTwinkle(false);
    assertFalse(test_controller->twinkle);
}


testF(NeopixelSinglePixel, test_twinkle_loop_brightness_up) {
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


class NeopixelSinglePixelBright: public aunit::TestOnce {
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


testF(NeopixelSinglePixelBright, test_twinkle_loop_brightness_down) {
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

void setup() {
    Serial.begin(115200);
#if defined(EPOXY_DUINO)
    Serial.setLineModeUnix();
#endif
}


void loop() {
    aunit::TestRunner::run();
}
