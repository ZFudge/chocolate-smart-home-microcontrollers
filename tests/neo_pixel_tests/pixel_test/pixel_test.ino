#line 2 "twinkle_test.ino"

#include <AUnitVerbose.h>
#include <chocolate-smart-home-microcontrollers.h>


using namespace NeoPixel;


Pixel *test_pixel;


class TestPixel: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_pixel = new Pixel;
        test_pixel->targetBrightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};


testF(TestPixel, test_targetBrightness) {
    assertEqual(test_pixel->targetBrightness, 255);
}


testF(TestPixel, test_setTargetBrightness) {
    test_pixel->setTargetBrightness(123);
    assertEqual(test_pixel->targetBrightness, 123);
}


testF(TestPixel, test_incrementBrightness_loops_up_and_loops_down) {
    assertEqual(test_pixel->brightness, 0);
    assertEqual(test_pixel->targetBrightness, 255);

    for (byte i = 0; i < 255; i++) {
        assertEqual(test_pixel->brightness, i);
        assertEqual(test_pixel->targetBrightness, 255);
        test_pixel->incrementBrightness();
    }

    assertEqual(test_pixel->brightness, 255);
    test_pixel->setTargetBrightness(0);

    for (byte i = 255; i > 0; i--) {
        assertEqual(test_pixel->brightness, i);
        assertEqual(test_pixel->targetBrightness, 0);
        test_pixel->incrementBrightness();
    }

    assertEqual(test_pixel->brightness, 0);
    assertEqual(test_pixel->targetBrightness, 0);
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
