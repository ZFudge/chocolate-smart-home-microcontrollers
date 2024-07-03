#line 2 "transform_test.ino"

#include <AUnitVerbose.h>

#include <chocolate-smart-home-microcontrollers.h>


using namespace NeoPixel;


#define TEST_DATA_PIN   6
#define TEST_NUM_PIX    10


class TestNeoPixelController {
protected:
    NeoPixelController test_controller;
    void test_controller_loop_n_times(unsigned int steps) {
        for (unsigned int i = 0; i < steps; i++)
            test_controller.loop();
    }
};

class TransformOff: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.setTransform(false);
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);
    }
};

testF(TransformOff, transform_false) {
    assertFalse(test_controller.transform);
}

testF(TransformOff, steps_not_set) {
    for (byte i = 0; i < TEST_NUM_PIX; i++) {
        Pixel *pixel = &test_controller.pixels[i];
        assertEqual(pixel->rTransformStep, 0.0);
        assertEqual(pixel->gTransformStep, 0.0);
        assertEqual(pixel->bTransformStep, 0.0);
    }
}

/*///////////////////////////////////////////////////////////////////////////*/
/*// Transform Start ////////////////////////////////////////////////////////*/
byte getColorIndex1(byte colorIndex) { return 1; };
byte getColorIndex0(byte colorIndex) { return 0; };
byte getColorIndex2(byte colorIndex) { return 2; };
byte getTransformStepsRemaining200() { return 200; };
class Transform: public aunit::TestOnce, public TestNeoPixelController {
protected:
    Pixel *pixel;
    void setup() override {
        aunit::TestOnce::setup();
        rgbs[0][0] = 0;
        rgbs[0][1] = 100;
        rgbs[0][2] = 255;

        rgbs[1][0] = 30;
        rgbs[1][1] = 200;
        rgbs[1][2] = 55;

        test_controller.init(TEST_DATA_PIN, 1);

        pixel = &test_controller.pixels[0];
        pixel->colorIndex = 0;
        pixel->setRGBFromIndex();
        pixel->getColorIndexRandom = getColorIndex1;
        pixel->getTransformStepsRemainingRandom = getTransformStepsRemaining200;
        pixel->setNewTransform();
    }
};

testF(Transform, steps_set) {
    assertNear(pixel->rTransformStep, 0.15, 0.006);
    assertNear(pixel->gTransformStep, 0.5, 0.006);
    assertNear(pixel->bTransformStep, -1.0, 0.006);
}

testF(Transform, rgb_steps_stepping) {
    assertEqual(pixel->r, 0.0);
    assertEqual(pixel->g, 100.0);
    assertEqual(pixel->b, 255.0);

    test_controller_loop_n_times(100);

    assertNear(pixel->r, 15.0, 0.006);
    assertNear(pixel->g, 150.0, 0.006);
    assertNear(pixel->b, 155.0, 0.006);
}

testF(Transform, pixel_step_stepping) {
    /* pixel.transformsStepsRemaining steps with each call to test_controller.loop */
    assertEqual(pixel->transformStepsRemaining, 200);
    for (byte i = 200; i > 100; i--) {
        assertEqual(pixel->transformStepsRemaining, i);
        test_controller.loop();
    }
    assertEqual(pixel->transformStepsRemaining, 100);
}

testF(Transform, cycle_interrupted) {
    // Assert starting color.
    assertEqual(pixel->r, 0.0);
    assertEqual(pixel->g, 100.0);
    assertEqual(pixel->b, 255.0);

    // Complete half of cycle.
    test_controller_loop_n_times(100);

    // Interrupt transform cycle.
    test_controller.setTransform(false);
    assertFalse(test_controller.transform);

    assertEqual(pixel->transformStepsRemaining, 100);

    // Cycle implicitly runs to completion when test_controller.transform is false.
    test_controller_loop_n_times(100);

    assertEqual(pixel->transformStepsRemaining, 0);
    assertNear(pixel->r, 30.0, 0.006);
    assertNear(pixel->g, 200.0, 0.006);
    assertNear(pixel->b, 55.0, 0.006);
}

/*// Transform End //////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////////////////////////////////////*/

class Transform__twinkle_off: public aunit::TestOnce, public TestNeoPixelController {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        rgbs[0][0] = 0;
        rgbs[0][1] = 100;
        rgbs[0][2] = 255;

        rgbs[1][0] = 37;
        rgbs[1][1] = 243;
        rgbs[1][2] = 249;

        test_controller.init(TEST_DATA_PIN, 2);

        test_controller.brightness = 255;
        test_controller.twinkle = false;
        test_controller.transform = true;

        test_controller.pixels[0].colorIndex = 0;
        test_controller.pixels[0].setRGBFromIndex();
        test_controller.pixels[0].getColorIndexRandom = getColorIndex1;
        test_controller.pixels[0].getTransformStepsRemainingRandom = getTransformStepsRemaining200;
        test_controller.pixels[0].setNewTransform();

        test_controller.pixels[1].colorIndex = 1;
        test_controller.pixels[1].setRGBFromIndex();
        test_controller.pixels[1].getColorIndexRandom = getColorIndex0;
        test_controller.pixels[1].getTransformStepsRemainingRandom = getTransformStepsRemaining200;
        test_controller.pixels[1].setNewTransform();
    }
};

testF(Transform__twinkle_off, color_swap) {
    Pixel *pixel_0 = &test_controller.pixels[0];
    Pixel *pixel_1 = &test_controller.pixels[1];

    assertNear(pixel_0->rTransformStep, 0.18, 0.006);
    assertNear(pixel_0->gTransformStep, 0.71, 0.006);
    assertNear(pixel_0->bTransformStep, -0.03, 0.006);

    assertNear(pixel_1->rTransformStep, -0.18, 0.006);
    assertNear(pixel_1->gTransformStep, -0.71, 0.006);
    assertNear(pixel_1->bTransformStep, 0.03, 0.006);

    assertNear(pixel_0->r, 0.0, 0.006);
    assertNear(pixel_0->g, 100.0, 0.006);
    assertNear(pixel_0->b, 255.0, 0.006);

    assertNear(pixel_1->r, 37.0, 0.006);
    assertNear(pixel_1->g, 243.0, 0.006);
    assertNear(pixel_1->b, 249.0, 0.006);

    test_controller_loop_n_times(200);

    assertNear(pixel_0->r, 37.0, 0.006);
    assertNear(pixel_0->g, 243.0, 0.006);
    assertNear(pixel_0->b, 249.0, 0.006);

    assertNear(pixel_1->r, 0.0, 0.006);
    assertNear(pixel_1->g, 100.0, 0.006);
    assertNear(pixel_1->b, 255.0, 0.006);
}

testF(Transform__twinkle_off, cycle_interrupted) {
    Pixel *pixel_0 = &test_controller.pixels[0];
    Pixel *pixel_1 = &test_controller.pixels[1];

    // Complete first half of cycle.
    test_controller_loop_n_times(100);
    // Interrupt test_controller.transform halfway through cycle.
    test_controller.setTransform(false);

    assertEqual(pixel_0->transformStepsRemaining, 100);
    assertEqual(pixel_1->transformStepsRemaining, 100);
    assertNear(pixel_0->r, 18.5, 0.006);
    assertNear(pixel_0->g, 171.5, 0.006);
    assertNear(pixel_0->b, 252.0, 0.006);
    assertNear(pixel_1->r, 18.5, 0.006);
    assertNear(pixel_1->g, 171.5, 0.006);
    assertNear(pixel_1->b, 252.0, 0.006);

    // Allow for implicit transform cycle settling.
    test_controller_loop_n_times(100);
    // Transform cycle ran to completion, to settle the color after setting
    // test_controller.transform to false.
    assertEqual(pixel_0->transformStepsRemaining, 0);
    assertNear(pixel_0->r, 37.0, 0.006);
    assertNear(pixel_0->g, 243.0, 0.006);
    assertNear(pixel_0->b, 249.0, 0.006);
    assertEqual(pixel_1->transformStepsRemaining, 0);
    assertNear(pixel_1->r, 0.0, 0.006);
    assertNear(pixel_1->g, 100.0, 0.006);
    assertNear(pixel_1->b, 255.0, 0.006);

    // After implicitly settling the transform cycle, test_controller.loop can
    // be called n times without any further calls to pixel.transform.
    test_controller_loop_n_times(1000);
    assertNear(pixel_0->r, 37.0, 0.006);
    assertNear(pixel_0->g, 243.0, 0.006);
    assertNear(pixel_0->b, 249.0, 0.006);
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
