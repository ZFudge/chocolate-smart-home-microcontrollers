#line 2 "neo_pixel_test.ino"

#include <AUnitVerbose.h>

#include <chocolate-smart-home-microcontrollers.h>


using namespace NeoPixel;


#define TEST_DATA_PIN   6
#define TEST_NUM_PIX    10


NeoPixelController *test_controller;


class NeoPixelDefault: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, TEST_NUM_PIX);
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};


testF(NeoPixelDefault, turn_off) {
    assertTrue(test_controller->on);
    test_controller->turnOnOff(false);
    assertFalse(test_controller->on);
}


testF(NeoPixelDefault, setBrightness) {
    test_controller->setBrightness(100);
    assertEqual(test_controller->brightness, 100);
    test_controller->setBrightness(255);
    assertEqual(test_controller->brightness, 255);
    test_controller->setBrightness(-10);
    assertEqual(test_controller->brightness, 246);
    test_controller->setBrightness(300);
    assertEqual(test_controller->brightness, 44);
}


class NeoPixelOff: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_controller->on = false;
        test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};


testF(NeoPixelOff, turn_on) {
    assertFalse(test_controller->on);
    test_controller->turnOnOff(true);
    assertTrue(test_controller->on);
}


test(neo_pixel_getRandomIndex) {
    assertNotEqual(Utils::getRandomIndex(1), 1);
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
