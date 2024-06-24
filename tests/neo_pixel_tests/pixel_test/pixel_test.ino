#line 2 "pixel_test.ino"

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


testF(TestPixel, test_setTargetBrightnessInRange) {
    test_pixel->setTargetBrightnessInRange(100, 101);
    assertEqual(test_pixel->targetBrightness, 100);
}


testF(TestPixel, test_pixel_loop_incrementBrightness_up) {
    assertEqual(test_pixel->brightness, 0);
    assertEqual(test_pixel->targetBrightness, 255);

    for (byte i = 0; i < 255; i++) {
        assertEqual(test_pixel->brightness, i);
        assertEqual(test_pixel->targetBrightness, 255);
        test_pixel->incrementBrightness();
    }
    assertEqual(test_pixel->brightness, 255);
}


class TestPixelBright: public aunit::TestOnce {
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


testF(TestPixelBright, test_pixel_loop_incrementBrightness_down) {
    assertEqual(test_pixel->brightness, 255);
    assertEqual(test_pixel->targetBrightness, 0);

    for (byte i = 255; i > 0; i--) {
        assertEqual(test_pixel->brightness, i);
        assertEqual(test_pixel->targetBrightness, 0);
        test_pixel->incrementBrightness();
    }
    assertEqual(test_pixel->brightness, 0);
}


class TestPixelMutable: public aunit::TestOnce {
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


testF(TestPixelMutable, test_pixel_mutate_brightness) {
    assertEqual(test_pixel->brightness, 0);
    assertEqual(test_pixel->targetBrightness, 0);
    test_pixel->twinkle(55);
    assertNotEqual(test_pixel->brightness, 0);
    assertNotEqual(test_pixel->targetBrightness, 0);
}


testF(TestPixelMutable, test_pixel_mutate_color) {
    assertEqual(test_pixel->r, 0);
    assertEqual(test_pixel->g, 127);
    assertEqual(test_pixel->b, 255);
    test_pixel->twinkle(55);
    assertNotEqual(test_pixel->colorIndex, 0);
    assertEqual(test_pixel->r, 77);
    assertEqual(test_pixel->g, 77);
    assertEqual(test_pixel->b, 77);
    assertLess(test_pixel->targetBrightness, 55);
}


class TestPixelBrightAndHighTargetBrightness: public aunit::TestOnce {
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


testF(TestPixelBrightAndHighTargetBrightness, test_reset_setTargetBrightness) {
    assertEqual(test_pixel->targetBrightness, 255);
    test_pixel->twinkle(20);
    assertLess(test_pixel->targetBrightness, 20);
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
