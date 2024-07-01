#line 2 "strip_test.ino"

#include <AUnitVerbose.h>

#include <chocolate-smart-home-microcontrollers.h>


using namespace NeoPixel;


#define TEST_DATA_PIN   6
#define TEST_NUM_PIX    10


NeoPixelController *test_controller;


class NeoPixelStripDefault: public aunit::TestOnce {
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

testF(NeoPixelStripDefault, getPin) {
    const byte pin = test_controller->strip.getPin();
    assertEqual(pin, TEST_DATA_PIN);
}

testF(NeoPixelStripDefault, numPixels) {
    const byte numPixels = test_controller->strip.numPixels();
    assertEqual(numPixels, TEST_NUM_PIX);
}


class NeoPixelStripRGBs: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, TEST_NUM_PIX);

        test_controller->pixels[0].brightness = 255;
        test_controller->pixels[1].brightness = 255;
        test_controller->pixels[2].brightness = 255;
        test_controller->pixels[3].brightness = 255;
        test_controller->pixels[4].brightness = 255;
        test_controller->pixels[5].brightness = 255;
        test_controller->pixels[6].brightness = 255;
        test_controller->pixels[7].brightness = 255;
        test_controller->pixels[8].brightness = 255;

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

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(NeoPixelStripRGBs, color) {
    Pixel *pixel;

    pixel = &test_controller->pixels[0];
    pixel->setColorIndex(0);
    pixel->setRGBFromIndex();
    test_controller->applyBrightnessAndOrRGBtoNeoPixel(0, pixel);
    assertEqual("2580", String(test_controller->strip.getPixelColor(0)));

    pixel = &test_controller->pixels[1];
    pixel->setColorIndex(1);
    pixel->setRGBFromIndex();
    test_controller->applyBrightnessAndOrRGBtoNeoPixel(1, pixel);
    assertEqual("1976370", String(test_controller->strip.getPixelColor(1)));

    pixel = &test_controller->pixels[2];
    pixel->setColorIndex(2);
    pixel->setRGBFromIndex();
    test_controller->applyBrightnessAndOrRGBtoNeoPixel(2, pixel);
    assertEqual("3950160", String(test_controller->strip.getPixelColor(2)));

    pixel = &test_controller->pixels[3];
    pixel->setColorIndex(3);
    pixel->setRGBFromIndex();
    test_controller->applyBrightnessAndOrRGBtoNeoPixel(3, pixel);
    assertEqual("5923950", String(test_controller->strip.getPixelColor(3)));

    pixel = &test_controller->pixels[4];
    pixel->setColorIndex(4);
    pixel->setRGBFromIndex();
    test_controller->applyBrightnessAndOrRGBtoNeoPixel(4, pixel);
    assertEqual("7897740", String(test_controller->strip.getPixelColor(4)));

    pixel = &test_controller->pixels[5];
    pixel->setColorIndex(5);
    pixel->setRGBFromIndex();
    test_controller->applyBrightnessAndOrRGBtoNeoPixel(5, pixel);
    assertEqual("9871530", String(test_controller->strip.getPixelColor(5)));

    pixel = &test_controller->pixels[6];
    pixel->setColorIndex(6);
    pixel->setRGBFromIndex();
    test_controller->applyBrightnessAndOrRGBtoNeoPixel(6, pixel);
    assertEqual("11845320", String(test_controller->strip.getPixelColor(6)));

    pixel = &test_controller->pixels[7];
    pixel->setColorIndex(7);
    pixel->setRGBFromIndex();
    test_controller->applyBrightnessAndOrRGBtoNeoPixel(7, pixel);
    assertEqual("13819110", String(test_controller->strip.getPixelColor(7)));

    pixel = &test_controller->pixels[8];
    pixel->setColorIndex(8);
    pixel->setRGBFromIndex();
    test_controller->applyBrightnessAndOrRGBtoNeoPixel(8, pixel);
    assertEqual("15792895", String(test_controller->strip.getPixelColor(8)));

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
