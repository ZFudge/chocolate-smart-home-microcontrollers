#line 2 "transform_test.ino"

#include <AUnitVerbose.h>

#include <chocolate-smart-home-microcontrollers.h>


using namespace NeoPixel;


#define TEST_DATA_PIN   6
#define TEST_NUM_PIX    10


const double ERROR_BOUND = 0.00075;

byte getColorIndex1(byte colorIndex) { return 1; };
byte getColorIndex0(byte colorIndex) { return 0; };
byte getColorIndex2(byte colorIndex) { return 2; };
byte getTransformStepsRemaining200() { return 200; };


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
    /* Idempotently call test_controller.setTransform with false */
    for (byte i = 0; i < 100; i++) {
        test_controller.setTransform(false);
        assertFalse(test_controller.transform);
    }
}

testF(TransformOff, transform_true) {
    /* Idempotently call test_controller.setTransform with true */
    for (byte i = 0; i < 100; i++) {
        test_controller.setTransform(true);
        assertTrue(test_controller.transform);
    }
}

testF(TransformOff, steps_not_set) {
    /* Transform steps should not be set when transform is off when
    controller.init is called. */
    for (byte i = 0; i < TEST_NUM_PIX; i++) {
        Pixel *pixel = &test_controller.pixels[i];
        assertEqual(pixel->rTransformStep, 0.0);
        assertEqual(pixel->gTransformStep, 0.0);
        assertEqual(pixel->bTransformStep, 0.0);
    }
}

/*///////////////////////////////////////////////////////////////////////////*/
/*// Transform Start ////////////////////////////////////////////////////////*/
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
        pixel->targetBrightness = 255;
        pixel->setRGBFromIndex();
        pixel->getColorIndexRandom = getColorIndex1;
        pixel->getTransformStepsRemainingRandom = getTransformStepsRemaining200;
        pixel->setNewTransform();
    }
};

testF(Transform, all_pixels_transform_cycles_are_current_false_by_the_fault) {
    assertFalse(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);
}

testF(Transform, all_pixels_transform_cycles_are_current_is_true_after_transform_turned_off_and_cycle_settled) {
    /* ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT should be true as soon as
    transform cycle(s) complete */
    test_controller.setTransform(false);
    test_controller_loop_n_times(199);

    assertFalse(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);
    test_controller.loop();
    assertTrue(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);
}

testF(Transform, all_pixels_transform_cycles_are_current_is_true_after_controller_turned_off_and_cycle_settled) {
    /* ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT should be true and remain true
    after the controller is turned off and current transform cycle settles,
    even though the controller is not fully dimmed yet and transform setting
    remains on. */

    // Complete 190/200 transform steps in the current cycle.
    assertEqual(pixel->transformStepsRemaining, 200);
    assertFalse(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);
    test_controller_loop_n_times(190);

    // Start dimming cycle before transform has settled.
    test_controller.turnOnOff(false);

    // Complete transform cycle.
    test_controller_loop_n_times(10);

    assertTrue(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);

    assertEqual(pixel->transformStepsRemaining, 0);
    assertEqual(pixel->brightness, 180);
    for (byte i = 0; i < 180; i++) {
        test_controller.loop();
        assertEqual(pixel->transformStepsRemaining, 0);
    }

    assertTrue(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);
}

testF(Transform, transform_cycles_settle_after_controller_turned_off) {
    /* Unsettled transform cycles should still settle after the controller has
    finished turning off. */
    test_controller_loop_n_times(20);
    assertEqual(pixel->brightness, 20);
    assertEqual(pixel->transformStepsRemaining, 180);

    test_controller.turnOnOff(false);

    test_controller_loop_n_times(20);
    assertEqual(pixel->brightness, 0);
    assertEqual(pixel->transformStepsRemaining, 160);

    test_controller_loop_n_times(160);
    assertEqual(pixel->brightness, 0);
    assertEqual(pixel->transformStepsRemaining, 0);
    assertNear(pixel->rTransformStep, 0.0, ERROR_BOUND);
    assertNear(pixel->gTransformStep, 0.0, ERROR_BOUND);
    assertNear(pixel->bTransformStep, 0.0, ERROR_BOUND);
    assertTrue(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);
}

testF(Transform, steps_set) {
    /* Transform steps should be set when transform is off when
    controller.init is called. */
    assertNear(pixel->rTransformStep, 0.15, ERROR_BOUND);
    assertNear(pixel->gTransformStep, 0.5, ERROR_BOUND);
    assertNear(pixel->bTransformStep, -1.0, ERROR_BOUND);
}

testF(Transform, steps_unset_when_transform_off) {
    /* Transform steps should not be set when transform is off when
    controller.init is called. */
    assertNear(pixel->rTransformStep, 0.15, ERROR_BOUND);
    assertNear(pixel->gTransformStep, 0.5, ERROR_BOUND);
    assertNear(pixel->bTransformStep, -1.0, ERROR_BOUND);
    test_controller_loop_n_times(199);
    test_controller.setTransform(false);
    test_controller_loop_n_times(1);
    assertNear(pixel->rTransformStep, 0.0, ERROR_BOUND);
    assertNear(pixel->gTransformStep, 0.0, ERROR_BOUND);
    assertNear(pixel->bTransformStep, 0.0, ERROR_BOUND);
}

testF(Transform, rgb_steps_stepping) {
    /* rgbs values should increment by the product of their transform step and
    the amount of calls to controller.loop. */
    assertEqual(pixel->r, 0.0);
    assertEqual(pixel->g, 100.0);
    assertEqual(pixel->b, 255.0);

    test_controller_loop_n_times(100);

    assertNear(pixel->r, 15.0, ERROR_BOUND);
    assertNear(pixel->g, 150.0, ERROR_BOUND);
    assertNear(pixel->b, 155.0, ERROR_BOUND);
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
    /* Transform cycle should implicitly run to completion when transform is
    turned off during transform cycle. */
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
    assertNear(pixel->r, 30.0, ERROR_BOUND);
    assertNear(pixel->g, 200.0, ERROR_BOUND);
    assertNear(pixel->b, 55.0, ERROR_BOUND);
}

testF(Transform, color_reset_during_cycle) {
    /* Changing color palette during transform cycle should result in new transform
    steps being set to transition between the current color state and the new
    target color, using the remaining amount of transform steps and the same color index.

        r starts at 0 with a target of 30
        200 transform steps in the cycle gives an r step of 0.15
        100 steps through the cycle, change the target r value to 115
        current r value is at 15 (halfway from 0 to 30)
        100 transform steps remaining to get to 115 gives a new r step of 1.0
    */
    // Starting color components.
    assertEqual(pixel->colorIndex, 1);
    assertEqual(pixel->r, 0.0);
    assertEqual(pixel->g, 100.0);
    assertEqual(pixel->b, 255.0);
    // Starting color component targets.
    assertEqual(rgbs[pixel->colorIndex][0], 30);
    assertEqual(rgbs[pixel->colorIndex][1], 200);
    assertEqual(rgbs[pixel->colorIndex][2], 55);
    // Starting transform steps.
    assertEqual(pixel->rTransformStep, 0.15);
    assertEqual(pixel->gTransformStep, 0.5);
    assertEqual(pixel->bTransformStep, -1.0);

    test_controller_loop_n_times(100);
    // Color is halfway through the original transition, before rgbs modified.
    assertNear(pixel->r, 15.0, ERROR_BOUND);
    assertNear(pixel->g, 150.0, ERROR_BOUND);
    assertNear(pixel->b, 155.0, ERROR_BOUND);

    // Interrupt transform cycle by updating color palette.
    // This triggers an update to the transform values.
    test_controller.updateRGBs("0,0,0,115,75,160,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0");
    // New transform steps from the current state to the new target color.
    assertEqual(pixel->colorIndex, 1);
    assertNear(pixel->rTransformStep, 1.0, ERROR_BOUND);
    assertNear(pixel->gTransformStep, -0.75, ERROR_BOUND);
    assertNear(pixel->bTransformStep, 0.05, ERROR_BOUND);

    test_controller_loop_n_times(50);
    // Color halfway through second transition, before rgbs modified.
    assertNear(pixel->r, 65.0, ERROR_BOUND);
    assertNear(pixel->g, 112.5, ERROR_BOUND);
    assertNear(pixel->b, 157.5, ERROR_BOUND);

    // Interrupt transform cycle again by updating color palette, again.
    // This triggers the second update to the transform values.
    test_controller.updateRGBs("0,0,0,1,100,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0");
    assertNear(pixel->rTransformStep, -1.28, ERROR_BOUND);
    assertNear(pixel->gTransformStep, -0.25, ERROR_BOUND);
    assertNear(pixel->bTransformStep, 1.95, ERROR_BOUND);
    test_controller_loop_n_times(50);
    // Ending color, as set by test_control.updateRGBs.
    assertNear(pixel->r, 1.0, ERROR_BOUND);
    assertNear(pixel->g, 100.0, ERROR_BOUND);
    assertNear(pixel->b, 255.0, ERROR_BOUND);
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

testF(Transform__twinkle_off, all_pixels_transform_cycles_are_current_is_true_after_controller_turned_off_and_cycle_settled) {
    /* ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT should be true and remain true
    after the controller is turned off and current transform cycle settles,
    even though the controller is not fully dimmed yet and transform setting
    remains on. */
    Pixel *pixel = &test_controller.pixels[0];

    // Complete 190/200 transform steps in the current cycle.
    assertEqual(pixel->transformStepsRemaining, 200);
    assertFalse(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);
    test_controller_loop_n_times(190);

    // Start dimming cycle before transform has settled.
    test_controller.turnOnOff(false);

    // Complete transform cycle.
    test_controller_loop_n_times(10);

    assertTrue(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);

    assertEqual(pixel->transformStepsRemaining, 0);
    assertEqual(pixel->brightness, 180);
    for (byte i = 0; i < 180; i++) {
        test_controller.loop();
        assertEqual(pixel->transformStepsRemaining, 0);
    }

    assertTrue(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);
}

testF(Transform__twinkle_off, color_swap) {
    /* Pixel colors should change to their target color after the controller
    transforms steps reach 0 */
    Pixel *pixel_0 = &test_controller.pixels[0];
    Pixel *pixel_1 = &test_controller.pixels[1];

    assertNear(pixel_0->rTransformStep, 0.185, ERROR_BOUND);
    assertNear(pixel_0->gTransformStep, 0.715, ERROR_BOUND);
    assertNear(pixel_0->bTransformStep, -0.03, ERROR_BOUND);

    assertNear(pixel_1->rTransformStep, -0.185, ERROR_BOUND);
    assertNear(pixel_1->gTransformStep, -0.715, ERROR_BOUND);
    assertNear(pixel_1->bTransformStep, 0.03, ERROR_BOUND);

    assertNear(pixel_0->r, 0.0, ERROR_BOUND);
    assertNear(pixel_0->g, 100.0, ERROR_BOUND);
    assertNear(pixel_0->b, 255.0, ERROR_BOUND);

    assertNear(pixel_1->r, 37.0, ERROR_BOUND);
    assertNear(pixel_1->g, 243.0, ERROR_BOUND);
    assertNear(pixel_1->b, 249.0, ERROR_BOUND);

    test_controller_loop_n_times(200);
    assertEqual(pixel_0->transformStepsRemaining, 0);

    assertNear(pixel_0->r, 37.0, ERROR_BOUND);
    assertNear(pixel_0->g, 243.0, ERROR_BOUND);
    assertNear(pixel_0->b, 249.0, ERROR_BOUND);

    assertNear(pixel_1->r, 0.0, ERROR_BOUND);
    assertNear(pixel_1->g, 100.0, ERROR_BOUND);
    assertNear(pixel_1->b, 255.0, ERROR_BOUND);
}

testF(Transform__twinkle_off, cycle_interrupted) {
    /* All pixel color transitions should run to completion even when
    controller.transform is turned off. */
    Pixel *pixel_0 = &test_controller.pixels[0];
    Pixel *pixel_1 = &test_controller.pixels[1];

    // Complete first half of cycle.
    test_controller_loop_n_times(100);
    // Interrupt test_controller.transform halfway through cycle.
    test_controller.setTransform(false);

    assertEqual(pixel_0->transformStepsRemaining, 100);
    assertEqual(pixel_1->transformStepsRemaining, 100);
    assertNear(pixel_0->r, 18.5, ERROR_BOUND);
    assertNear(pixel_0->g, 171.5, ERROR_BOUND);
    assertNear(pixel_0->b, 252.0, ERROR_BOUND);
    assertNear(pixel_1->r, 18.5, ERROR_BOUND);
    assertNear(pixel_1->g, 171.5, ERROR_BOUND);
    assertNear(pixel_1->b, 252.0, ERROR_BOUND);

    // Allow for implicit transform cycle settling.
    test_controller_loop_n_times(100);
    // Transform cycle ran to completion, to settle the color after setting
    // test_controller.transform to false.
    assertEqual(pixel_0->transformStepsRemaining, 0);
    assertNear(pixel_0->r, 37.0, ERROR_BOUND);
    assertNear(pixel_0->g, 243.0, ERROR_BOUND);
    assertNear(pixel_0->b, 249.0, ERROR_BOUND);
    assertEqual(pixel_1->transformStepsRemaining, 0);
    assertNear(pixel_1->r, 0.0, ERROR_BOUND);
    assertNear(pixel_1->g, 100.0, ERROR_BOUND);
    assertNear(pixel_1->b, 255.0, ERROR_BOUND);

    // After implicitly settling the transform cycle, test_controller.loop can
    // be called n times without any further calls to pixel.transform.
    test_controller_loop_n_times(1000);
    assertNear(pixel_0->r, 37.0, ERROR_BOUND);
    assertNear(pixel_0->g, 243.0, ERROR_BOUND);
    assertNear(pixel_0->b, 249.0, ERROR_BOUND);
}


class TransformToSameComponents: public aunit::TestOnce, public TestNeoPixelController {
protected:
    Pixel *pixel;
    void setup() override {
        aunit::TestOnce::setup();
        rgbs[0][0] = 0;
        rgbs[0][1] = 100;
        rgbs[0][2] = 55;

        rgbs[1][0] = 0;
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

testF(TransformToSameComponents, same_color_components) {
    /* rgbs steps should have a value of 0.0 when value is equal to the target color component. */
    assertEqual(pixel->r, 0.0);
    assertEqual(pixel->g, 100.0);
    assertEqual(pixel->b, 55.0);

    assertEqual(pixel->rTransformStep, 0.0);
    assertEqual(pixel->gTransformStep, 0.5);
    assertEqual(pixel->bTransformStep, 0.0);

    test_controller_loop_n_times(100);

    assertEqual(pixel->rTransformStep, 0.0);
    assertEqual(pixel->gTransformStep, 0.5);
    assertEqual(pixel->bTransformStep, 0.0);
    assertEqual(pixel->r, 0.0);
    assertEqual(pixel->g, 150.0);
    assertEqual(pixel->b, 55.0);

    test_controller_loop_n_times(100);

    assertEqual(pixel->rTransformStep, 0.0);
    assertEqual(pixel->gTransformStep, 0.5);
    assertEqual(pixel->bTransformStep, 0.0);
    assertEqual(pixel->r, 0.0);
    assertEqual(pixel->g, 200.0);
    assertEqual(pixel->b, 55.0);
}


class TransformOffTwinkleOffControllerOff: public aunit::TestOnce, public TestNeoPixelController {
protected:
    Pixel *pixel;
    void setup() override {
        aunit::TestOnce::setup();
        rgbs[0][0] = 0;
        rgbs[0][1] = 100;
        rgbs[0][2] = 255;

        test_controller.init(TEST_DATA_PIN, 1);
        test_controller.on = false;
        test_controller.twinkle = false;
        test_controller.transform = false;

        pixel = &test_controller.pixels[0];
        pixel->colorIndex = 0;
        pixel->setRGBFromIndex();
    }
};

testF(TransformOffTwinkleOffControllerOff, updateRGBs_no_transform_when_controller_off) {
    /* pixel rgbs should instantaneously change to the corresponding color of
    the same color index in the new palette. */
    assertNear(pixel->r, 0.0, ERROR_BOUND);
    assertNear(pixel->g, 100.0, ERROR_BOUND);
    assertNear(pixel->b, 255.0, ERROR_BOUND);

    test_controller.updateRGBs("115,75,160,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0");

    assertNear(pixel->r, 115.0, ERROR_BOUND);
    assertNear(pixel->g, 75.0, ERROR_BOUND);
    assertNear(pixel->b, 160.0, ERROR_BOUND);
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
