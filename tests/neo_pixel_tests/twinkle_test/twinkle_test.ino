#line 2 "twinkle_test.ino"

#include <AUnitVerbose.h>
#include <chocolate-smart-home-microcontrollers.h>


using namespace NeoPixel;


#define TEST_DATA_PIN   6


NeoPixelController *test_controller;


class NeopixelSinglePixel: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, 1);
        Pixel *pixel = &test_controller->pixels[0];
        pixel->setTargetBrightness(100);
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


testF(NeopixelSinglePixel, test_twinkle_loop) {
    Pixel *pixel = &test_controller->pixels[0];

    assertEqual(pixel->brightness, 0);
    assertEqual(pixel->targetBrightness, 100);

    for (byte i = 0; i < 20; i++) {
        assertEqual(pixel->brightness, i);
        test_controller->loop();
    }
    assertEqual(pixel->brightness, 20);
}


class NeopixelSinglePixelBright: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, 1);
        Pixel *pixel = &test_controller->pixels[0];
        pixel->brightness = 255;
        pixel->setTargetBrightness(200);
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};


testF(NeopixelSinglePixelBright, test_twinkle_loop) {
    Pixel *pixel = &test_controller->pixels[0];

    for (byte i = 255; i > 200; i--) {
        assertEqual(pixel->brightness, i);
        test_controller->loop();
    }
    assertEqual(pixel->brightness, 200);
    assertEqual(pixel->targetBrightness, 200);

    test_controller->loop();
    assertNotEqual(pixel->targetBrightness, 200);
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
