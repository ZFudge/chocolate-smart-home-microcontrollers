#line 2 "interaction_test.ino"

#include <AUnitVerbose.h>

#include <chocolate-smart-home-microcontrollers.h>


using namespace NeoPixel;


#define TEST_DATA_PIN   6
#define TEST_NUM_PIX    10

const double ERROR_BOUND = 0.00075;


byte getTransformStepsRemaining200() { return 200; };
byte getTransformStepsRemaining100() { return 100; };
byte getTransformStepsRemaining50() { return 50; };
byte getTargetBrightnessInRange140(const byte, const byte) { return 140; };
byte getTargetBrightnessInRange111(const byte, const byte) { return 111; };
byte getTargetBrightnessInRange79(const byte, const byte) { return 79; };
byte getTargetBrightnessInRange20(const byte, const byte) { return 20; };
byte getColorIndex1(byte colorIndex) { return 1; };
byte getColorIndex2(byte colorIndex) { return 2; };
byte getColorIndex0(byte colorIndex) { return 0; };


class TestNeoPixelController {
protected:
    NeoPixelController test_controller;
    void test_controller_loop_n_times(unsigned int steps) {
        for (unsigned int i = 0; i < steps; i++)
            test_controller.loop();
    }
};


class Simultaneous: public aunit::TestOnce, public TestNeoPixelController {
protected:
    Pixel *p_1, *p_2, *p_3;
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

        test_controller.init(TEST_DATA_PIN, 3);
        test_controller.brightness = 150;

        p_1 = &test_controller.pixels[0];
        p_2 = &test_controller.pixels[1];
        p_3 = &test_controller.pixels[2];

        p_1->brightness = 100;
        p_1->targetBrightness = 200;
        p_1->colorIndex = 0;
        p_1->setRGBFromIndex();
        p_1->getTargetBrightnessInRange = getTargetBrightnessInRange140;
        p_1->getColorIndexRandom = getColorIndex1;
        p_1->getTransformStepsRemainingRandom = getTransformStepsRemaining200;
        p_1->setNewTransform();
        p_1->getColorIndexRandom = getColorIndex2;

        p_2->brightness = 110;
        p_2->targetBrightness = 99;
        p_2->colorIndex = 1;
        p_2->getTargetBrightnessInRange = getTargetBrightnessInRange111;
        p_2->getColorIndexRandom = getColorIndex2;
        p_2->getTransformStepsRemainingRandom = getTransformStepsRemaining200;
        p_2->setNewTransform();
        p_2->getColorIndexRandom = getColorIndex0;

        p_3->brightness = 90;
        p_3->targetBrightness = 101;
        p_3->colorIndex = 2;
        p_3->getTargetBrightnessInRange = getTargetBrightnessInRange79;
        p_3->getColorIndexRandom = getColorIndex0;
        p_3->getTransformStepsRemainingRandom = getTransformStepsRemaining200;
        p_3->setNewTransform();
        p_3->getColorIndexRandom = getColorIndex1;
    }
};

testF(Simultaneous, interactions) {
    /* Run through several different controller settings, testing that the
    different settings work as expected when changed during various controller
    states.
    Intended to best simulate the control flow of natural use. */

    ///////////////////////////////////////////////////////////////////////////
    /* Simultaneously test twinkle and transform cycles as well as brightening
    from a dimmed state. */
    assertTrue(test_controller.twinkle);
    assertTrue(test_controller.transform);
    assertEqual(test_controller.brightness, 150);
    assertNear(p_1->rTransformStep, 0.15, ERROR_BOUND);
    assertNear(p_1->gTransformStep, 0.15, ERROR_BOUND);
    assertNear(p_1->bTransformStep, 0.15, ERROR_BOUND);
    assertNear(p_2->rTransformStep, 0.15, ERROR_BOUND);
    assertNear(p_2->gTransformStep, 0.15, ERROR_BOUND);
    assertNear(p_2->bTransformStep, 0.15, ERROR_BOUND);
    assertNear(p_3->rTransformStep, -0.3, ERROR_BOUND);
    assertNear(p_3->gTransformStep, -0.3, ERROR_BOUND);
    assertNear(p_3->bTransformStep, -0.3, ERROR_BOUND);

    test_controller_loop_n_times(5);

    /* Pixel One */
    assertEqual(p_1->brightness, 105);
    assertEqual(p_1->targetBrightness, 140);
    assertNear(p_1->r, 0.75, ERROR_BOUND);
    assertNear(p_1->g, 10.75, ERROR_BOUND);
    assertNear(p_1->b, 20.75, ERROR_BOUND);
    // Test RGB values on strip NeoPixel.
    long pixelColor = test_controller.strip.getPixelColor(0);
    assertEqual(0, byte(pixelColor >> 16));
    assertEqual(3, byte(pixelColor >> 8 & 255));
    assertEqual(5, byte(pixelColor & 255));
    /* Pixel One End */
    /* Pixel Two Start */
    assertEqual(p_2->brightness, 105);
    assertEqual(p_2->targetBrightness, 99);
    assertNear(p_2->r, 30.75, ERROR_BOUND);
    assertNear(p_2->g, 40.75, ERROR_BOUND);
    assertNear(p_2->b, 50.75, ERROR_BOUND);
    // Test RGB values on strip NeoPixel.
    pixelColor = test_controller.strip.getPixelColor(1);
    assertEqual(7, byte(pixelColor >> 16));
    assertEqual(10, byte(pixelColor >> 8 & 255));
    assertEqual(12, byte(pixelColor & 255));
    /* Pixel Two End */
    /* Pixel Three Start */
    assertEqual(p_3->brightness, 95);
    assertEqual(p_3->targetBrightness, 101);
    assertNear(p_3->r, 58.5, ERROR_BOUND);
    assertNear(p_3->g, 68.5, ERROR_BOUND);
    assertNear(p_3->b, 78.5, ERROR_BOUND);
    // Test RGB values on strip NeoPixel.
    pixelColor = test_controller.strip.getPixelColor(2);
    assertEqual(13, byte(pixelColor >> 16));
    assertEqual(15, byte(pixelColor >> 8 & 255));
    assertEqual(17, byte(pixelColor & 255));
    /* Pixel Three End */

    ///////////////////////////////////////////////////////////////////////////
    /* Test settling of twinkle cycles after interrupt by turning twinkle
    setting off.
    Test transform cycles uneffected by twinkling interrupt. */
    assertTrue(test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    test_controller.setTwinkle(false);
    assertFalse(test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);

    test_controller_loop_n_times(5);

    assertEqual(p_1->brightness, 110);
    assertNear(p_1->r, 1.5, ERROR_BOUND);
    assertNear(p_1->g, 11.5, ERROR_BOUND);
    assertNear(p_1->b, 21.5, ERROR_BOUND);
    assertEqual(p_2->brightness, 110);
    assertNear(p_2->r, 31.5, ERROR_BOUND);
    assertNear(p_2->g, 41.5, ERROR_BOUND);
    assertNear(p_2->b, 51.5, ERROR_BOUND);
    assertEqual(p_3->brightness, 100);
    assertNear(p_3->r, 57.0, ERROR_BOUND);
    assertNear(p_3->g, 67.0, ERROR_BOUND);
    assertNear(p_3->b, 77.0, ERROR_BOUND);
    // Transform cycles still stepping.
    test_controller.loop();
    assertNear(p_1->r, 1.65, ERROR_BOUND);
    assertNear(p_2->g, 41.65, ERROR_BOUND);
    assertNear(p_3->b, 76.7, ERROR_BOUND);

    ///////////////////////////////////////////////////////////////////////////
    /* Test transforms cycles settling after turning off transform. */
    assertFalse(test_controller.twinkle);
    assertTrue(test_controller.transform);
    assertFalse(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);
    test_controller.setTransform(false);

    test_controller_loop_n_times(4);

    // Transform cycles still stepping.
    assertEqual(p_1->brightness, 115);
    assertNear(p_1->r, 2.25, ERROR_BOUND);
    assertNear(p_1->g, 12.25, ERROR_BOUND);
    assertNear(p_1->b, 22.25, ERROR_BOUND);
    assertEqual(p_2->brightness, 115);
    assertNear(p_2->r, 32.25, ERROR_BOUND);
    assertNear(p_2->g, 42.25, ERROR_BOUND);
    assertNear(p_2->b, 52.25, ERROR_BOUND);
    assertEqual(p_3->brightness, 105);
    assertNear(p_3->r, 55.5, ERROR_BOUND);
    assertNear(p_3->g, 65.5, ERROR_BOUND);
    assertNear(p_3->b, 75.5, ERROR_BOUND);

    assertFalse(test_controller.twinkle);
    assertFalse(test_controller.transform);
    // Test that step values and RGB values are correct after running all
    // transform cycles to completion.
    test_controller_loop_n_times(185);
    assertTrue(test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);

    assertEqual(p_1->brightness, 150);
    assertEqual(p_2->brightness, 150);
    assertEqual(p_3->brightness, 150);
    assertNear(p_1->rTransformStep, 0.0, ERROR_BOUND);
    assertNear(p_1->gTransformStep, 0.0, ERROR_BOUND);
    assertNear(p_1->bTransformStep, 0.0, ERROR_BOUND);
    assertNear(p_2->rTransformStep, 0.0, ERROR_BOUND);
    assertNear(p_2->gTransformStep, 0.0, ERROR_BOUND);
    assertNear(p_2->bTransformStep, 0.0, ERROR_BOUND);
    assertNear(p_3->rTransformStep, 0.0, ERROR_BOUND);
    assertNear(p_3->gTransformStep, 0.0, ERROR_BOUND);
    assertNear(p_3->bTransformStep, 0.0, ERROR_BOUND);
    assertNear(p_1->r, 30.0, ERROR_BOUND);
    assertNear(p_1->g, 40.0, ERROR_BOUND);
    assertNear(p_1->b, 50.0, ERROR_BOUND);
    assertNear(p_2->r, 60.0, ERROR_BOUND);
    assertNear(p_2->g, 70.0, ERROR_BOUND);
    assertNear(p_2->b, 80.0, ERROR_BOUND);
    assertNear(p_3->r, 0.0, ERROR_BOUND);
    assertNear(p_3->g, 10.0, ERROR_BOUND);
    assertNear(p_3->b, 20.0, ERROR_BOUND);

    assertFalse(test_controller.twinkle);
    assertFalse(test_controller.transform);
    /* Test that twinkle and transform setters are idempotent. */
    test_controller.setTwinkle(false);
    test_controller.setTransform(false);
    assertFalse(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);

    test_controller_loop_n_times(1000);

    assertTrue(test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    assertEqual(p_1->brightness, 150);
    assertNear(p_1->rTransformStep, 0.0, ERROR_BOUND);
    assertNear(p_2->gTransformStep, 0.0, ERROR_BOUND);
    assertNear(p_3->bTransformStep, 0.0, ERROR_BOUND);
    assertNear(p_1->r, 30.0, ERROR_BOUND);
    assertNear(p_2->g, 70.0, ERROR_BOUND);
    assertNear(p_3->b, 20.0, ERROR_BOUND);

    ///////////////////////////////////////////////////////////////////////////
    /* Test that dimming cycle works when twinkle and transform settings are off. */
    assertTrue(test_controller.on);
    assertFalse(test_controller.twinkle);
    assertFalse(test_controller.transform);
    test_controller.turnOnOff(false);

    test_controller_loop_n_times(149);

    assertEqual(test_controller.brightness, 150);
    assertFalse(test_controller.on);
    assertEqual(p_1->brightness, 1);
    assertEqual(p_2->brightness, 1);
    assertEqual(p_3->brightness, 1);
    assertNear(p_1->rTransformStep, 0.0, ERROR_BOUND);
    assertNear(p_2->gTransformStep, 0.0, ERROR_BOUND);
    assertNear(p_3->bTransformStep, 0.0, ERROR_BOUND);
    assertNear(p_1->r, 30.0, ERROR_BOUND);
    assertNear(p_2->g, 70.0, ERROR_BOUND);
    assertNear(p_3->b, 20.0, ERROR_BOUND);
    assertFalse(test_controller.twinkle);
    assertFalse(test_controller.transform);
    pixelColor = test_controller.strip.getPixelColor(0);
    assertEqual(0, byte(pixelColor >> 16));
    assertEqual(0, byte(pixelColor >> 8 & 255));
    assertEqual(0, byte(pixelColor & 255));
    pixelColor = test_controller.strip.getPixelColor(1);
    assertEqual(0, byte(pixelColor >> 16));
    assertEqual(0, byte(pixelColor >> 8 & 255));
    assertEqual(0, byte(pixelColor & 255));
    pixelColor = test_controller.strip.getPixelColor(2);
    assertEqual(0, byte(pixelColor >> 16));
    assertEqual(0, byte(pixelColor >> 8 & 255));
    assertEqual(0, byte(pixelColor & 255));

    test_controller.loop();
    assertEqual(p_1->brightness, 0);
    assertEqual(p_2->brightness, 0);
    assertEqual(p_3->brightness, 0);

    /* Test that the controller loop is idempotent when controller is off. */
    assertFalse(test_controller.on);
    test_controller_loop_n_times(1000);

    assertEqual(test_controller.brightness, 150);
    assertFalse(test_controller.on);
    assertEqual(p_1->brightness, 0);
    assertEqual(p_2->brightness, 0);
    assertEqual(p_3->brightness, 0);
    assertNear(p_1->rTransformStep, 0.0, ERROR_BOUND);
    assertNear(p_2->gTransformStep, 0.0, ERROR_BOUND);
    assertNear(p_3->bTransformStep, 0.0, ERROR_BOUND);
    assertNear(p_1->r, 30.0, ERROR_BOUND);
    assertNear(p_2->g, 70.0, ERROR_BOUND);
    assertNear(p_3->b, 20.0, ERROR_BOUND);
    pixelColor = test_controller.strip.getPixelColor(0);
    assertEqual(0, byte(pixelColor >> 16));
    assertEqual(0, byte(pixelColor >> 8 & 255));
    assertEqual(0, byte(pixelColor & 255));
    pixelColor = test_controller.strip.getPixelColor(1);
    assertEqual(0, byte(pixelColor >> 16));
    assertEqual(0, byte(pixelColor >> 8 & 255));
    assertEqual(0, byte(pixelColor & 255));
    pixelColor = test_controller.strip.getPixelColor(2);
    assertEqual(0, byte(pixelColor >> 16));
    assertEqual(0, byte(pixelColor >> 8 & 255));
    assertEqual(0, byte(pixelColor & 255));

    ///////////////////////////////////////////////////////////////////////////
    /* Test that brightening cycle works when twinkle and transform settings
    are off, the twinkle is turned on partway through, an transform turned on
    at the end of the brightening cycle. */
    test_controller.turnOnOff(true);

    test_controller_loop_n_times(75);

    assertEqual(test_controller.brightness, 150);
    assertEqual(p_1->brightness, 75);
    assertEqual(p_2->brightness, 75);
    assertEqual(p_3->brightness, 75);
    pixelColor = test_controller.strip.getPixelColor(0);
    assertEqual(5, byte(pixelColor >> 16));
    assertEqual(7, byte(pixelColor >> 8 & 255));
    assertEqual(9, byte(pixelColor & 255));
    pixelColor = test_controller.strip.getPixelColor(1);
    assertEqual(10, byte(pixelColor >> 16));
    assertEqual(12, byte(pixelColor >> 8 & 255));
    assertEqual(14, byte(pixelColor & 255));
    pixelColor = test_controller.strip.getPixelColor(2);
    assertEqual(0, byte(pixelColor >> 16));
    assertEqual(2, byte(pixelColor >> 8 & 255));
    assertEqual(3, byte(pixelColor & 255));

    // Twinkle on.
    test_controller.setTwinkle(true);
    assertEqual(p_1->targetBrightness, 140);
    assertEqual(p_2->targetBrightness, 99);
    assertEqual(p_3->targetBrightness, 101);

    p_1->getTargetBrightnessInRange = getTargetBrightnessInRange111;
    p_2->getTargetBrightnessInRange = getTargetBrightnessInRange140;
    p_3->getTargetBrightnessInRange = getTargetBrightnessInRange79;

    test_controller_loop_n_times(24);
    assertEqual(p_1->brightness, 99);
    assertEqual(p_2->brightness, 99);
    assertEqual(p_3->brightness, 99);
    assertEqual(p_1->targetBrightness, 140);
    assertEqual(p_2->targetBrightness, 99);
    assertEqual(p_3->targetBrightness, 101);

    test_controller.loop();
    assertEqual(p_2->targetBrightness, 140);
    p_2->getTargetBrightnessInRange = getTargetBrightnessInRange20;

    test_controller.loop();
    assertEqual(p_1->brightness, 101);
    assertEqual(p_2->brightness, 101);
    assertEqual(p_3->brightness, 101);
    assertEqual(p_1->targetBrightness, 140);
    assertEqual(p_2->targetBrightness, 140);
    assertEqual(p_3->targetBrightness, 101);

    test_controller.loop();
    assertEqual(p_1->brightness, 102);
    assertEqual(p_2->brightness, 102);
    assertEqual(p_3->brightness, 100);
    assertEqual(p_1->targetBrightness, 140);
    assertEqual(p_2->targetBrightness, 140);
    assertEqual(p_3->targetBrightness, 79);
    p_3->getTargetBrightnessInRange = getTargetBrightnessInRange20;

    // Transform on.
    assertTrue(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);
    test_controller.setTransform(true);
    assertFalse(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);
    p_1->getTransformStepsRemainingRandom = getTransformStepsRemaining100;
    p_2->getTransformStepsRemainingRandom = getTransformStepsRemaining100;
    p_3->getTransformStepsRemainingRandom = getTransformStepsRemaining100;
    assertEqual(p_1->colorIndex, 1);
    assertEqual(p_2->colorIndex, 2);
    assertEqual(p_3->colorIndex, 0);

    test_controller.loop();
    assertEqual(p_1->colorIndex, 2);
    assertEqual(p_2->colorIndex, 0);
    assertEqual(p_3->colorIndex, 1);
    assertNear(p_1->rTransformStep, 0.3, ERROR_BOUND);
    assertNear(p_2->gTransformStep, -0.6, ERROR_BOUND);
    assertNear(p_3->bTransformStep, 0.3, ERROR_BOUND);
    assertNear(p_1->r, 30.3, ERROR_BOUND);
    assertNear(p_1->g, 40.3, ERROR_BOUND);
    assertNear(p_1->b, 50.3, ERROR_BOUND);
    assertNear(p_2->r, 59.4, ERROR_BOUND);
    assertNear(p_2->g, 69.4, ERROR_BOUND);
    assertNear(p_2->b, 79.4, ERROR_BOUND);
    assertNear(p_3->r, 0.3, ERROR_BOUND);
    assertNear(p_3->g, 10.3, ERROR_BOUND);
    assertNear(p_3->b, 20.3, ERROR_BOUND);
    test_controller_loop_n_times(37);
    assertEqual(p_1->brightness, 140);
    assertEqual(p_2->brightness, 140);
    assertEqual(p_3->brightness, 62);
    assertNear(p_1->r, 41.4, ERROR_BOUND);
    assertNear(p_1->g, 51.4, ERROR_BOUND);
    assertNear(p_1->b, 61.4, ERROR_BOUND);
    assertNear(p_2->r, 37.2, ERROR_BOUND);
    assertNear(p_2->g, 47.2, ERROR_BOUND);
    assertNear(p_2->b, 57.2, ERROR_BOUND);
    assertNear(p_3->r, 11.4, ERROR_BOUND);
    assertNear(p_3->g, 21.4, ERROR_BOUND);
    assertNear(p_3->b, 31.4, ERROR_BOUND);
    assertEqual(p_1->targetBrightness, 140);
    assertEqual(p_2->targetBrightness, 140);
    assertEqual(p_3->targetBrightness, 20);

    test_controller.loop();
    assertEqual(p_1->brightness, 139);
    assertEqual(p_2->brightness, 139);
    assertEqual(p_3->brightness, 61);
    assertEqual(p_1->targetBrightness, 111);
    assertEqual(p_2->targetBrightness, 20);
    assertEqual(p_3->targetBrightness, 20);

    ///////////////////////////////////////////////////////////////////////////
    /* Test dimming cycle when twinkle and transform are both on. */ 
    test_controller.turnOnOff(false);
    assertTrue(test_controller.twinkle);
    assertTrue(test_controller.transform);
    test_controller_loop_n_times(61);
    assertEqual(p_3->brightness, 0);
    test_controller_loop_n_times(78);
    assertEqual(p_1->brightness, 0);
    assertEqual(p_2->brightness, 0);
    assertEqual(p_3->brightness, 0);
    assertNear(p_1->r, 60.0, ERROR_BOUND);
    assertNear(p_2->g, 10.0, ERROR_BOUND);
    assertNear(p_3->b, 50.0, ERROR_BOUND);

    ///////////////////////////////////////////////////////////////////////////
    /* Test turning twinkle and transform off with controller already off, then
    turning controller on. */
    test_controller.setTwinkle(false);
    assertFalse(test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    test_controller.setTransform(false);
    assertFalse(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);
    test_controller.turnOnOff(true);

    test_controller_loop_n_times(1000);
    assertTrue(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);
    assertEqual(p_1->brightness, 150);
    assertEqual(p_2->brightness, 150);
    assertEqual(p_3->brightness, 150);
    assertNear(p_1->r, 60.0, ERROR_BOUND);
    assertNear(p_2->g, 10.0, ERROR_BOUND);
    assertNear(p_3->b, 50.0, ERROR_BOUND);
    assertNear(p_1->rTransformStep, 0.0, ERROR_BOUND);
    assertNear(p_2->gTransformStep, 0.0, ERROR_BOUND);
    assertNear(p_3->bTransformStep, 0.0, ERROR_BOUND);

    p_1->getTransformStepsRemainingRandom = getTransformStepsRemaining50;
    p_2->getTransformStepsRemainingRandom = getTransformStepsRemaining50;
    p_3->getTransformStepsRemainingRandom = getTransformStepsRemaining50;
    p_1->getColorIndexRandom = getColorIndex0;
    p_2->getColorIndexRandom = getColorIndex1;
    p_3->getColorIndexRandom = getColorIndex2;

    ///////////////////////////////////////////////////////////////////////////
    /* Turn transform on, from non-transitive controller state. */
    assertTrue(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);
    test_controller.setTransform(true);
    assertFalse(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);

    test_controller.loop();

    assertEqual(p_1->colorIndex, 0);
    assertEqual(p_2->colorIndex, 1);
    assertEqual(p_3->colorIndex, 2);
    assertNear(p_1->rTransformStep, -1.2, ERROR_BOUND);
    assertNear(p_2->gTransformStep, 0.6, ERROR_BOUND);
    assertNear(p_3->bTransformStep, 0.6, ERROR_BOUND);
    assertNear(p_1->r, 58.8, ERROR_BOUND);
    assertNear(p_2->g, 10.6, ERROR_BOUND);
    assertNear(p_3->b, 50.6, ERROR_BOUND);

    assertFalse(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);
    test_controller.setTransform(false);
    assertFalse(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);

    test_controller_loop_n_times(49);

    assertNear(p_1->r, 0.0, ERROR_BOUND);
    assertNear(p_2->g, 40.0, ERROR_BOUND);
    assertNear(p_3->b, 80.0, ERROR_BOUND);


    test_controller.setBrightness(222);
    test_controller_loop_n_times(72);
    assertEqual(p_1->brightness, 222);
    assertEqual(p_2->brightness, 222);
    assertEqual(p_3->brightness, 222);
    assertTrue(test_controller.ALL_PIXELS_TRANSFORM_CYCLES_ARE_CURRENT);

    // Implicitly turn controller off using low brightness setting
    assertTrue(test_controller.on);
    test_controller.setBrightness(4);
    assertFalse(test_controller.on);
    assertEqual(test_controller.brightness, 222);


}


void setup() {
    Serial.begin(115200);
    aunit::TestRunner::exclude("*");
    aunit::TestRunner::include("Simultaneous*");
#if defined(EPOXY_DUINO)
    Serial.setLineModeUnix();
#endif
}


void loop() {
    aunit::TestRunner::run();
}
