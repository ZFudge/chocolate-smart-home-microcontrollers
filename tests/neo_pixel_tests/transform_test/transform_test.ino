#line 2 "transform_test.ino"

#include <AUnitVerbose.h>

#include <chocolate-smart-home-microcontrollers.h>


using namespace NeoPixel;


#define TEST_DATA_PIN   6
#define TEST_NUM_PIX    10


NeoPixelController *test_controller;


class TransformOff: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->setTransform(false);
        test_controller->init(TEST_DATA_PIN, TEST_NUM_PIX);
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(TransformOff, steps_set) {
    assertFalse(test_controller->transform);

    for (byte i = 0; i < TEST_NUM_PIX; i++) {
        Pixel *pixel = &test_controller->pixels[i];
        assertEqual(pixel->rTransformStep, 0.0);
        assertEqual(pixel->gTransformStep, 0.0);
        assertEqual(pixel->bTransformStep, 0.0);
    }
}


byte getColorIndex1(byte colorIndex) {
    return 1;
};
byte getColorIndex0(byte colorIndex) {
    return 0;
};

byte getTransformStepsRemaining200() {
    return 200;
};

class Transform: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        rgbs[0][0] = 0;
        rgbs[0][1] = 100;
        rgbs[0][2] = 255;

        rgbs[1][0] = 30;
        rgbs[1][1] = 200;
        rgbs[1][2] = 55;

        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, 1);

        test_controller->pixels[0].colorIndex = 0;
        test_controller->pixels[0].setRGBFromIndex();
        test_controller->pixels[0].getColorIndexRandom = getColorIndex1;
        test_controller->pixels[0].getTransformStepsRemainingRandom = getTransformStepsRemaining200;
        test_controller->pixels[0].setNewTransform();
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Transform, steps_set) {
    Pixel *pixel = &test_controller->pixels[0];
    assertEqual(String(pixel->rTransformStep), "0.15");
    assertEqual(String(pixel->gTransformStep), "0.50");
    assertEqual(String(pixel->bTransformStep), "-1.00");
}


class Transform__twinkle_off: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        rgbs[0][0] = 0;
        rgbs[0][1] = 100;
        rgbs[0][2] = 255;

        rgbs[1][0] = 37;
        rgbs[1][1] = 243;
        rgbs[1][2] = 249;

        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, 2);

        test_controller->brightness = 255;
        test_controller->twinkle = false;
        test_controller->transform = true;

        test_controller->pixels[0].colorIndex = 0;
        test_controller->pixels[0].setRGBFromIndex();
        test_controller->pixels[0].getColorIndexRandom = getColorIndex1;
        test_controller->pixels[0].getTransformStepsRemainingRandom = getTransformStepsRemaining200;
        test_controller->pixels[0].setNewTransform();

        test_controller->pixels[1].colorIndex = 1;
        test_controller->pixels[1].setRGBFromIndex();
        test_controller->pixels[1].getColorIndexRandom = getColorIndex0;
        test_controller->pixels[1].getTransformStepsRemainingRandom = getTransformStepsRemaining200;
        test_controller->pixels[1].setNewTransform();
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Transform__twinkle_off, color_swap) {
    Pixel *pixel_0 = &test_controller->pixels[0];
    Pixel *pixel_1 = &test_controller->pixels[1];

    assertEqual(String(pixel_0->rTransformStep), "0.18");
    assertEqual(String(pixel_0->gTransformStep), "0.71");
    assertEqual(String(pixel_0->bTransformStep), "-0.03");

    assertEqual(String(pixel_1->rTransformStep), "-0.18");
    assertEqual(String(pixel_1->gTransformStep), "-0.71");
    assertEqual(String(pixel_1->bTransformStep), "0.03");

    assertEqual(String(pixel_0->r), "0.00");
    assertEqual(String(pixel_0->g), "100.00");
    assertEqual(String(pixel_0->b), "255.00");

    assertEqual(String(pixel_1->r), "37.00");
    assertEqual(String(pixel_1->g), "243.00");
    assertEqual(String(pixel_1->b), "249.00");

    for (byte i = 0; i < 200; i++)
        test_controller->loop();

    assertEqual(String(pixel_0->r), "37.00");
    assertEqual(String(pixel_0->g), "243.00");
    assertEqual(String(pixel_0->b), "249.00");

    assertEqual(String(pixel_1->r), "-0.00");
    assertEqual(String(pixel_1->g), "100.00");
    assertEqual(String(pixel_1->b), "255.00");
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
