#line 2 "strip_test.ino"

#include <AUnitVerbose.h>

#include <chocolate-smart-home-microcontrollers.h>


using namespace NeoPixel;


#define TEST_DATA_PIN   6
#define TEST_NUM_PIX    10


class NeoPixelStripDefault: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);
    }
};

testF(NeoPixelStripDefault, getPin) {
    assertEqual(TEST_DATA_PIN, test_controller.strip.getPin());
}

testF(NeoPixelStripDefault, numPixels) {
    assertEqual(TEST_NUM_PIX, test_controller.strip.numPixels());
}


class NeoPixelStripRGBs: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    Pixel *pixel;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);

        test_controller.pixels[0].brightness = 255;
        test_controller.pixels[1].brightness = 255;
        test_controller.pixels[2].brightness = 255;
        test_controller.pixels[3].brightness = 255;
        test_controller.pixels[4].brightness = 255;
        test_controller.pixels[5].brightness = 255;
        test_controller.pixels[6].brightness = 255;
        test_controller.pixels[7].brightness = 255;
        test_controller.pixels[8].brightness = 255;

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

testF(NeoPixelStripRGBs, color) {
    pixel = &test_controller.pixels[0];
    pixel->setColorIndex(0);
    pixel->setRGBFromIndex();
    test_controller.applyPixelSettingsToNeoPixel(0, pixel);
    assertEqual(uint32_t(2580), test_controller.strip.getPixelColor(0));

    pixel = &test_controller.pixels[1];
    pixel->setColorIndex(1);
    pixel->setRGBFromIndex();
    test_controller.applyPixelSettingsToNeoPixel(1, pixel);
    assertEqual(uint32_t(1976370), test_controller.strip.getPixelColor(1));

    pixel = &test_controller.pixels[2];
    pixel->setColorIndex(2);
    pixel->setRGBFromIndex();
    test_controller.applyPixelSettingsToNeoPixel(2, pixel);
    assertEqual(uint32_t(3950160), test_controller.strip.getPixelColor(2));

    pixel = &test_controller.pixels[3];
    pixel->setColorIndex(3);
    pixel->setRGBFromIndex();
    test_controller.applyPixelSettingsToNeoPixel(3, pixel);
    assertEqual(uint32_t(5923950), test_controller.strip.getPixelColor(3));

    pixel = &test_controller.pixels[4];
    pixel->setColorIndex(4);
    pixel->setRGBFromIndex();
    test_controller.applyPixelSettingsToNeoPixel(4, pixel);
    assertEqual(uint32_t(7897740), test_controller.strip.getPixelColor(4));

    pixel = &test_controller.pixels[5];
    pixel->setColorIndex(5);
    pixel->setRGBFromIndex();
    test_controller.applyPixelSettingsToNeoPixel(5, pixel);
    assertEqual(uint32_t(9871530), test_controller.strip.getPixelColor(5));

    pixel = &test_controller.pixels[6];
    pixel->setColorIndex(6);
    pixel->setRGBFromIndex();
    test_controller.applyPixelSettingsToNeoPixel(6, pixel);
    assertEqual(uint32_t(11845320), test_controller.strip.getPixelColor(6));

    pixel = &test_controller.pixels[7];
    pixel->setColorIndex(7);
    pixel->setRGBFromIndex();
    test_controller.applyPixelSettingsToNeoPixel(7, pixel);
    assertEqual(uint32_t(13819110), test_controller.strip.getPixelColor(7));

    pixel = &test_controller.pixels[8];
    pixel->setColorIndex(8);
    pixel->setRGBFromIndex();
    test_controller.applyPixelSettingsToNeoPixel(8, pixel);
    assertEqual(uint32_t(15792895), test_controller.strip.getPixelColor(8));
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
