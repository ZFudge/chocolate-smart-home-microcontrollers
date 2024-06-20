#line 2 "on_test.ino"

#include <AUnitVerbose.h>
#include <chocolate-smart-home-microcontrollers.h>


using namespace NeoPixel;


#define TEST_DATA_PIN   6
#define TEST_NUM_PIX    10


NeoPixelController *test_controller;


class NeoPixelDefaults: public aunit::TestOnce {
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


testF(NeoPixelDefaults, test_setBrightness) {
    test_controller->setBrightness(100);
    assertEqual(test_controller->brightness, 100);
    test_controller->setBrightness(255);
    assertEqual(test_controller->brightness, 255);
    test_controller->setBrightness(-10);
    assertEqual(test_controller->brightness, 246);
    test_controller->setBrightness(300);
    assertEqual(test_controller->brightness, 44);
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
