#line 2 "twinkle_test.ino"

#include <AUnitVerbose.h>
#include <chocolate-smart-home-microcontrollers.h>


using namespace NeoPixel;


#define TEST_DATA_PIN   6
#define TEST_NUM_PIX    10


NeoPixelController *test_controller;


class NeopixelDefaults: public aunit::TestOnce {
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


testF(NeopixelDefaults, test_setTwinkle_default) {
    assertTrue(test_controller->twinkle);
}


testF(NeopixelDefaults, test_setTwinkle_false) {
    test_controller->setTwinkle(false);
    assertFalse(test_controller->twinkle);
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
