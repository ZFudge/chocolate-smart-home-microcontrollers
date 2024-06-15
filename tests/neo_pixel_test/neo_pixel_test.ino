#line 2 "neopixel_test.ino"

#include <AUnitVerbose.h>
#include <chocolate-smart-home-microcontrollers.h>

using namespace Neopixel;


#define TEST_DATA_PIN   6
#define TEST_NUM_PIX    10


NeoPixelController test_controller;


test(test_numPixels) {
    const byte numPixels = test_controller.strip.numPixels();
    assertEqual(numPixels, TEST_NUM_PIX);
}


test(test_getPin) {
    const byte pin = test_controller.strip.getPin();
    assertEqual(pin, TEST_DATA_PIN);
}


void setup() {
    Serial.begin(115200);
#if defined(EPOXY_DUINO)
    Serial.setLineModeUnix();
#endif
    test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);
}


void loop() {
    aunit::TestRunner::run();
}