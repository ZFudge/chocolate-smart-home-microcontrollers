#line 2 "pixel_test.ino"

#include <AUnitVerbose.h>

#include <chocolate-smart-home-microcontrollers.h>


using namespace NeoPixel;


const double ERROR_BOUND = 0.00075;

byte getColorIndex1(byte colorIndex) { return 1; };
byte getTransformStepsRemaining200() { return 200; };


Pixel *test_pixel;


class _Pixel: public aunit::TestOnce {
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

testF(_Pixel, targetBrightness) {
    assertEqual(test_pixel->targetBrightness, 255);
}

testF(_Pixel, setTargetBrightness) {
    test_pixel->setTargetBrightness(123);
    assertEqual(test_pixel->targetBrightness, 123);
}

testF(_Pixel, setTargetBrightnessInRange) {
    test_pixel->setTargetBrightnessInRange(100, 101);
    assertEqual(test_pixel->targetBrightness, 100);
}

testF(_Pixel, setColorRandom) {
    const byte index_1 = test_pixel->colorIndex;
    test_pixel->setColorRandom();
    const byte index_2 = test_pixel->colorIndex;
    assertNotEqual(index_1, index_2);
    assertEqual(byte(test_pixel->r), rgbs[index_2][0]);
    assertEqual(byte(test_pixel->g), rgbs[index_2][1]);
    assertEqual(byte(test_pixel->b), rgbs[index_2][2]);
}

testF(_Pixel, setColorRandomAny) {
    test_pixel->setColorRandomAny();
    const byte index = test_pixel->colorIndex;
    assertEqual(byte(test_pixel->r), rgbs[index][0]);
    assertEqual(byte(test_pixel->g), rgbs[index][1]);
    assertEqual(byte(test_pixel->b), rgbs[index][2]);
}


testF(_Pixel, pixel_loop_incrementBrightness_up) {
    assertEqual(test_pixel->brightness, 0);
    assertEqual(test_pixel->targetBrightness, 255);

    for (byte i = 0; i < 255; i++) {
        assertEqual(test_pixel->brightness, i);
        assertEqual(test_pixel->targetBrightness, 255);
        test_pixel->incrementBrightness();
    }
    assertEqual(test_pixel->brightness, 255);
}


class Neo_PixelBright: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_pixel = new Pixel;
        test_pixel->brightness = 255;
        test_pixel->targetBrightness = 0;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};


testF(Neo_PixelBright, pixel_loop_incrementBrightness_down) {
    assertEqual(test_pixel->brightness, 255);
    assertEqual(test_pixel->targetBrightness, 0);

    for (byte i = 255; i > 0; i--) {
        assertEqual(test_pixel->brightness, i);
        assertEqual(test_pixel->targetBrightness, 0);
        test_pixel->incrementBrightness();
    }
    assertEqual(test_pixel->brightness, 0);
}


class Neo_PixelMutable: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_pixel = new Pixel;
        test_pixel->brightness = 0;
        test_pixel->targetBrightness = 0;
        rgbs[0][0] = 0;
        rgbs[0][1] = 127;
        rgbs[0][2] = 255;
        // Fill out remaining color components with all 77s.
        for (byte i = 1; i < 9; i++)
            for (byte ii = 0; ii < 3; ii++)
                rgbs[i][ii] = 77;
        test_pixel->colorIndex = 0;
        test_pixel->r = rgbs[0][0];
        test_pixel->g = rgbs[0][1];
        test_pixel->b = rgbs[0][2];
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};


testF(Neo_PixelMutable, pixel_mutate_brightness) {
    assertEqual(test_pixel->brightness, 0);
    assertEqual(test_pixel->targetBrightness, 0);
    test_pixel->twinkle(55, false);
    assertNotEqual(test_pixel->brightness, 0);
    assertNotEqual(test_pixel->targetBrightness, 0);
}


testF(Neo_PixelMutable, pixel_mutate_color) {
    assertEqual(test_pixel->r, 0.0);
    assertEqual(test_pixel->g, 127.0);
    assertEqual(test_pixel->b, 255.0);
    test_pixel->twinkle(55, false);
    assertNotEqual(test_pixel->colorIndex, 0);
    assertEqual(test_pixel->r, 77.0);
    assertEqual(test_pixel->g, 77.0);
    assertEqual(test_pixel->b, 77.0);
    assertLess(test_pixel->targetBrightness, 55);
}


class Neo_PixelBrightAndHighTargetBrightness: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_pixel = new Pixel;
        test_pixel->brightness = 255;
        test_pixel->targetBrightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};


testF(Neo_PixelBrightAndHighTargetBrightness, reset_setTargetBrightness) {
    assertEqual(test_pixel->targetBrightness, 255);
    test_pixel->twinkle(20, false);
    assertLess(test_pixel->targetBrightness, 20);
}


class Transform: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        rgbs[0][0] = 0;
        rgbs[0][1] = 127;
        rgbs[0][2] = 255;
        rgbs[1][0] = 123;
        rgbs[1][1] = 55;
        rgbs[1][2] = 200;
        test_pixel = new Pixel;
        test_pixel->colorIndex = 0;
        test_pixel->getColorIndexRandom = getColorIndex1;
        test_pixel->getTransformStepsRemainingRandom = getTransformStepsRemaining200;
    }

    void teardown() override {

        aunit::TestOnce::teardown();
    }
};


testF(Transform, setNewTransform) {
    test_pixel->setNewTransform();

    assertNear(test_pixel->rTransformStep, 0.615, ERROR_BOUND);
    assertNear(test_pixel->gTransformStep, -0.36, ERROR_BOUND);
    assertNear(test_pixel->bTransformStep, -0.275, ERROR_BOUND);
}


testF(Transform, setColorIndex) {
    test_pixel->setColorIndex(0);
    assertEqual(test_pixel->colorIndex, 0);
    test_pixel->setColorIndex(1);
    assertEqual(test_pixel->colorIndex, 1);
    test_pixel->setColorIndex(2);
    assertEqual(test_pixel->colorIndex, 2);
    test_pixel->setColorIndex(3);
    assertEqual(test_pixel->colorIndex, 3);
    test_pixel->setColorIndex(4);
    assertEqual(test_pixel->colorIndex, 4);
    test_pixel->setColorIndex(5);
    assertEqual(test_pixel->colorIndex, 5);
    test_pixel->setColorIndex(6);
    assertEqual(test_pixel->colorIndex, 6);
    test_pixel->setColorIndex(7);
    assertEqual(test_pixel->colorIndex, 7);
    test_pixel->setColorIndex(8);
    assertEqual(test_pixel->colorIndex, 8);
}


testF(Transform, setColorIndex_bound_to_palette_length) {
    test_pixel->setColorIndex(7);
    assertEqual(test_pixel->colorIndex, 7);
    test_pixel->setColorIndex(100);
    assertEqual(test_pixel->colorIndex, 7);
}


class PixelColorIndexSet: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    Pixel *test_pixel;
    void setup() override {
        aunit::TestOnce::setup();
        rgbs[0][0] = 0;
        rgbs[0][1] = 1;
        rgbs[0][2] = 2;
        rgbs[1][0] = 3;
        rgbs[1][1] = 4;
        rgbs[1][2] = 5;
        test_controller.init(6, 1);
        test_pixel = &test_controller.pixels[0];
        test_pixel->colorIndex = 0;
        test_pixel->setRGBFromIndex();
    }
};

testF(PixelColorIndexSet, setRGBFromIndex) {
    assertNear(test_pixel->r, 0.0, ERROR_BOUND);
    assertNear(test_pixel->g, 1.0, ERROR_BOUND);
    assertNear(test_pixel->b, 2.0, ERROR_BOUND);
    test_pixel->colorIndex = 1;
    test_pixel->setRGBFromIndex();
    assertNear(test_pixel->r, 3.0, ERROR_BOUND);
    assertNear(test_pixel->g, 4.0, ERROR_BOUND);
    assertNear(test_pixel->b, 5.0, ERROR_BOUND);
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
