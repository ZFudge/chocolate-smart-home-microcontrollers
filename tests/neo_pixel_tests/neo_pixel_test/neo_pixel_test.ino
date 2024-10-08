#line 2 "neo_pixel_test.ino"

#include <AUnitVerbose.h>

#include <chocolate-smart-home-microcontrollers.h>


using namespace NeoPixel;


#define TEST_DATA_PIN   6
#define TEST_NUM_PIX    10


test(neo_pixel_getRandomIndex) {
    assertNotEqual(Utils::getRandomIndex(1), 1);
}

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


test(high_number_of_pixels) {
    NeoPixelController test_controller_100;
    test_controller_100.init(6, 100);

    NeoPixelController test_controller_200;
    test_controller_200.init(7, 200);

    NeoPixelController test_controller_255;
    test_controller_255.init(8, 255);

    for (byte i = 0; i < 10; i++) {
        test_controller_100.loop();
        test_controller_200.loop();
        test_controller_255.loop();
    }
}


test(idempotent_init) {
    NeoPixelController test_controller;
    for (byte i = 0; i < 10; i++)
        test_controller.init(6, 10);
    assertEqual(6, test_controller.strip.getPin());
    assertEqual(10, test_controller.strip.numPixels());
}


test(init_updateType) {
    NeoPixelController test_controller;
    test_controller.init(6, 50, NEO_GRB + NEO_KHZ800);
    for (int i = 0; i < 1000; i++)
        test_controller.loop();
}


test(init_zero_numOfPixels) {
    /* Passing controller.init a numOfPixels value of 0 should trigger a guard
    clause that aborts the controller.init from initializing the controller */
    NeoPixelController test_controller;
    test_controller.init(6, 0);
    assertEqual(0, test_controller.numOfPixels);
    assertEqual(0, test_controller.strip.numPixels());
    assertEqual(-1, test_controller.strip.getPin());
    assertEqual(NULL, test_controller.pixels);
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

testF(NeoPixelDefault, implicitly_turn_off) {
    /* Controller should be implicitly turned off when brightness is set
    to a low enough setting, leaving the former brightness level intact */
    assertTrue(test_controller.on);
    assertEqual(test_controller.brightness, 255);

    test_controller.setBrightness(4);

    assertFalse(test_controller.on);
    assertEqual(test_controller.brightness, 255);
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

testF(NeoPixelDefault, applyBrightness) {
    assertEqual(test_controller.applyBrightness(255, 255), 255);
    assertEqual(test_controller.applyBrightness(255, 127), 127);
    assertEqual(test_controller.applyBrightness(255, 85 ), 85);
    assertEqual(test_controller.applyBrightness(255, 0  ), 0);
    assertEqual(test_controller.applyBrightness(0,   255), 0);
    assertEqual(test_controller.applyBrightness(0,   0  ), 0);
    assertEqual(test_controller.applyBrightness(100, 0  ), 0);
    assertEqual(test_controller.applyBrightness(100, 127), 50);
    assertEqual(test_controller.applyBrightness(100, 85 ), 33);
}

testF(NeoPixelDefault, setMS) {
    assertEqual(test_controller.ms, 0);
    test_controller.setMS(10);
    assertEqual(test_controller.ms, 10);
    test_controller.setMS(255);
    assertEqual(test_controller.ms, 255);
    test_controller.setMS(127);
    assertEqual(test_controller.ms, 127);
    test_controller.setMS(4);
    assertEqual(test_controller.ms, 4);
    test_controller.setMS(1);
    assertEqual(test_controller.ms, 1);
    test_controller.setMS(0);
    assertEqual(test_controller.ms, 0);
    test_controller.setMS(7);
    assertEqual(test_controller.ms, 7);
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


class RGBs: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        rgbs[0][0] = 0;
        rgbs[0][1] = 10;
        rgbs[0][2] = 20;
        rgbs[1][0] = 30;
        rgbs[1][1] = 40;
        rgbs[1][2] = 50;
        rgbs[2][0] = 60;
        rgbs[2][1] = 70;
        rgbs[2][2] = 80;
        rgbs[3][0] = 90;
        rgbs[3][1] = 100;
        rgbs[3][2] = 110;
        rgbs[4][0] = 120;
        rgbs[4][1] = 130;
        rgbs[4][2] = 140;
        rgbs[5][0] = 150;
        rgbs[5][1] = 160;
        rgbs[5][2] = 170;
        rgbs[6][0] = 180;
        rgbs[6][1] = 190;
        rgbs[6][2] = 200;
        rgbs[7][0] = 210;
        rgbs[7][1] = 220;
        rgbs[7][2] = 230;
        rgbs[8][0] = 240;
        rgbs[8][1] = 250;
        rgbs[8][2] = 255;
    }
};

testF(RGBs, test_controller_updateRGBs) {
    assertEqual(rgbs[0][0], 0);
    assertEqual(rgbs[3][2], 110);
    assertEqual(rgbs[6][2], 200);
    assertEqual(rgbs[8][1], 250);

    test_controller.updateRGBs("1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27");

    assertEqual(rgbs[0][0], 1);
    assertEqual(rgbs[0][1], 2);
    assertEqual(rgbs[0][2], 3);

    assertEqual(rgbs[1][0], 4);
    assertEqual(rgbs[1][1], 5);
    assertEqual(rgbs[1][2], 6);

    assertEqual(rgbs[2][0], 7);
    assertEqual(rgbs[2][1], 8);
    assertEqual(rgbs[2][2], 9);

    assertEqual(rgbs[3][0], 10);
    assertEqual(rgbs[3][1], 11);
    assertEqual(rgbs[3][2], 12);

    assertEqual(rgbs[4][0], 13);
    assertEqual(rgbs[4][1], 14);
    assertEqual(rgbs[4][2], 15);

    assertEqual(rgbs[5][0], 16);
    assertEqual(rgbs[5][1], 17);
    assertEqual(rgbs[5][2], 18);

    assertEqual(rgbs[6][0], 19);
    assertEqual(rgbs[6][1], 20);
    assertEqual(rgbs[6][2], 21);

    assertEqual(rgbs[7][0], 22);
    assertEqual(rgbs[7][1], 23);
    assertEqual(rgbs[7][2], 24);

    assertEqual(rgbs[8][0], 25);
    assertEqual(rgbs[8][1], 26);
    assertEqual(rgbs[8][2], 27);
}


class NeoPixelMaxCount: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.setMaxCount(2);
        test_controller.init(TEST_DATA_PIN, 4);
    }
};

testF(NeoPixelMaxCount, setMaxCount) {
    /* setMaxCount should not modify maxCount if controller is already initialized */
    test_controller.setMaxCount(7);
    assertEqual(2, test_controller.maxCount);
}

testF(NeoPixelMaxCount, numOfPixels) {
    assertEqual(2, test_controller.numOfPixels);
}

testF(NeoPixelMaxCount, strip_numOfPixels) {
    assertEqual(4, test_controller.strip.numPixels());
}

test(setMaxCount_zero) {
    /* controller.setMaxCount should not set controller.maxCount to 0 */
    NeoPixelController test_controller;
    test_controller.setMaxCount(0);
    assertEqual(test_controller.maxCount, 50);
}

test(setMaxCount) {
    NeoPixelController test_controller;
    test_controller.setMaxCount(14);
    assertEqual(test_controller.maxCount, 14);
    test_controller.setMaxCount(99);
    assertEqual(test_controller.maxCount, 99);
    test_controller.setMaxCount(255);
    assertEqual(test_controller.maxCount, 255);
}


class NeoPixelMaxCountHighNeoPixelCount: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.setMaxCount(75);
        test_controller.init(TEST_DATA_PIN, 300);
    }
};

testF(NeoPixelMaxCountHighNeoPixelCount, strip_numOfPixels) {
    assertEqual(300, test_controller.strip.numPixels());
}

testF(NeoPixelMaxCountHighNeoPixelCount, colors_match) {
    // assertEqual(300, test_controller.strip.numPixels());
    // Test RGB values on strip NeoPixel.
    uint32_t pixelColor;

    for (byte l = 0; l < 4; l++) {
        test_controller.loop();
        for (int i = 0; i < test_controller.maxCount; i++) {
            pixelColor = test_controller.strip.getPixelColor(i);
            assertEqual(pixelColor, test_controller.strip.getPixelColor(i + 75));
            assertEqual(pixelColor, test_controller.strip.getPixelColor(i + 150));
            assertEqual(pixelColor, test_controller.strip.getPixelColor(i + 225));
        }
    }
}


class RGBsController: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        rgbs[0][0] = 0;
        rgbs[0][1] = 0;
        rgbs[0][2] = 0;
        rgbs[1][0] = 0;
        rgbs[1][1] = 0;
        rgbs[1][2] = 0;
        rgbs[2][0] = 0;
        rgbs[2][1] = 0;
        rgbs[2][2] = 0;
        rgbs[3][0] = 0;
        rgbs[3][1] = 0;
        rgbs[3][2] = 0;
        rgbs[4][0] = 0;
        rgbs[4][1] = 0;
        rgbs[4][2] = 0;
        rgbs[5][0] = 0;
        rgbs[5][1] = 0;
        rgbs[5][2] = 0;
        rgbs[6][0] = 0;
        rgbs[6][1] = 0;
        rgbs[6][2] = 0;
        rgbs[7][0] = 0;
        rgbs[7][1] = 0;
        rgbs[7][2] = 0;
        rgbs[8][0] = 0;
        rgbs[8][1] = 0;
        rgbs[8][2] = 0;
        test_controller.twinkle = false;
        test_controller.transform = false;
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);
        for (int i = 0; i < TEST_NUM_PIX; i++) {
            test_controller.pixels[i].r = 0;
            test_controller.pixels[i].g = 0;
            test_controller.pixels[i].b = 0;
        }
    }
};

testF(RGBsController, test_controller_updateRGBs__no_twinkle_no_transform) {
    for (int i = 0; i < 255; i++)
        test_controller.loop();

    test_controller.updateRGBs("33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33");

    for (int i = 0; i < 200; i++)
        test_controller.loop();

    for (int i = 0; i < TEST_NUM_PIX; i++)
        assertEqual(test_controller.strip.getPixelColor(i), (uint32_t)2171169);
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
