#line 2 "neo_pixel_test.ino"

#include <AUnitVerbose.h>

#include <chocolate-smart-home-microcontrollers.h>


using namespace NeoPixel;


#define TEST_DATA_PIN   6
#define TEST_NUM_PIX    10


test(two_controllers) {
    NeoPixelController test_controller_0;
    test_controller_0.init(6, 2);
    assertEqual(6, test_controller_0.strip.getPin());
    assertEqual(2, test_controller_0.strip.numPixels());

    NeoPixelController test_controller_1;
    test_controller_1.init(7, 3);
    assertEqual(7, test_controller_1.strip.getPin());
    assertEqual(3, test_controller_1.strip.numPixels());
}


class NeoPixelDefault: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);
    }
};

testF(NeoPixelDefault, turn_off) {
    assertTrue(test_controller.on);
    test_controller.turnOnOff(false);
    assertFalse(test_controller.on);
}

testF(NeoPixelDefault, setBrightness) {
    test_controller.setBrightness(100);
    assertEqual(test_controller.brightness, 100);
    test_controller.setBrightness(255);
    assertEqual(test_controller.brightness, 255);
    test_controller.setBrightness(-10);
    assertEqual(test_controller.brightness, 246);
    test_controller.setBrightness(300);
    assertEqual(test_controller.brightness, 44);
}


class NeoPixelOff: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_controller.on = false;
        test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;
    }
};

testF(NeoPixelOff, turn_on) {
    assertFalse(test_controller.on);
    test_controller.turnOnOff(true);
    assertTrue(test_controller.on);
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
