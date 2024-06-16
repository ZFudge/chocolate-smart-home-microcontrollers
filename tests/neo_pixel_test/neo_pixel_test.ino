#line 2 "neopixel_test.ino"

#include <AUnitVerbose.h>
#include <chocolate-smart-home-microcontrollers.h>

using namespace NeoPixel;


#define TEST_DATA_PIN   6
#define TEST_NUM_PIX    10


NeoPixelController *test_controller;


class NeoPixelDefaultSettings: public aunit::TestOnce {
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


testF(NeoPixelDefaultSettings, test_getPin) {
    const byte pin = test_controller->strip.getPin();
    assertEqual(pin, TEST_DATA_PIN);
}


testF(NeoPixelDefaultSettings, test_numPixels) {
    const byte numPixels = test_controller->strip.numPixels();
    assertEqual(numPixels, TEST_NUM_PIX);
}


testF(NeoPixelDefaultSettings, test_on) {
    assertTrue(test_controller->on);
}


testF(NeoPixelDefaultSettings, test_turn_off) {
    test_controller->turnOnOff(false);
    assertFalse(test_controller->on);
}


class NeoPixelOff: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_controller->turnOnOff(false);
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};


testF(NeoPixelOff, test_off) {
    assertFalse(test_controller->on);
}


testF(NeoPixelOff, test_turn_on) {
    test_controller->turnOnOff(true);
    assertTrue(test_controller->on);
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
