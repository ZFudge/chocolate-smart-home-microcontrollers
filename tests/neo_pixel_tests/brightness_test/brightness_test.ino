#line 2 "brightness_test.ino"

#include <AUnitVerbose.h>

#include <chocolate-smart-home-microcontrollers.h>


using namespace NeoPixel;


#define TEST_DATA_PIN   6
#define TEST_NUM_PIX    10


NeoPixelController *test_controller;


/* Test that pixel.brightness is automatically adjusted to the correct level,
 * or range, when the controller.brightness setting is changed.
 */


/* START brightness adjusts downward */

class Brightness__ControllerOn__Twinkle__Transform__BrightenedPixels: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, TEST_NUM_PIX);

        test_controller->twinkle = true;
        test_controller->transform = true;

        test_controller->brightness = 50;
        test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller->pixels[i].brightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOn__Twinkle__Transform__BrightenedPixels, brightness_down) {
    assertEqual(test_controller->brightness, 50);
    for (byte brightness = 255; brightness >= 50; brightness--) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller->pixels[i].brightness, brightness);
        }
        test_controller->loop();
    }
}


class Brightness__ControllerOn__Twinkle__TransformOff__BrightenedPixels: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, TEST_NUM_PIX);

        test_controller->twinkle = true;
        test_controller->transform = false;

        test_controller->brightness = 50;
        test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller->pixels[i].brightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOn__Twinkle__TransformOff__BrightenedPixels, brightness_down) {
    assertEqual(test_controller->brightness, 50);
    for (byte brightness = 255; brightness >= 50; brightness--) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller->pixels[i].brightness, brightness);
        }
        test_controller->loop();
    }
}


class Brightness__ControllerOn__TwinkleOff__Transform__BrightenedPixels: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, TEST_NUM_PIX);

        test_controller->twinkle = false;
        test_controller->transform = true;

        test_controller->brightness = 50;
        test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller->pixels[i].brightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOn__TwinkleOff__Transform__BrightenedPixels, brightness_down) {
    assertEqual(test_controller->brightness, 50);
    for (byte brightness = 255; brightness >= 50; brightness--) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller->pixels[i].brightness, brightness);
        }
        test_controller->loop();
    }
}


class Brightness__ControllerOn__TwinkleOff__TransformOff__BrightenedPixels: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, TEST_NUM_PIX);

        test_controller->twinkle = false;
        test_controller->transform = false;

        test_controller->brightness = 50;
        test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller->pixels[i].brightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOn__TwinkleOff__TransformOff__BrightenedPixels, brightness_down) {
    assertEqual(test_controller->brightness, 50);
    for (byte brightness = 255; brightness >= 50; brightness--) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller->pixels[i].brightness, brightness);
        }
        test_controller->loop();
    }
}


class Brightness__ControlleOff_Twinkle_Transform__BrightenedPixels: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_controller->on = false;

        test_controller->twinkle = true;
        test_controller->transform = true;

        test_controller->brightness = 50;
        test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller->pixels[i].brightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControlleOff_Twinkle_Transform__BrightenedPixels, brightness_down) {
    assertEqual(test_controller->brightness, 50);
    for (byte brightness = 255; brightness > 0; brightness--) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller->pixels[i].brightness, brightness);
        }
        test_controller->loop();
    }
}


class Brightness__ControllerOff_TwinkleOff_Transform__BrightenedPixels: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_controller->on = false;

        test_controller->twinkle = false;
        test_controller->transform = true;

        test_controller->brightness = 50;
        test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller->pixels[i].brightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOff_TwinkleOff_Transform__BrightenedPixels, brightness_down) {
    assertEqual(test_controller->brightness, 50);
    for (byte brightness = 255; brightness > 0; brightness--) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller->pixels[i].brightness, brightness);
        }
        test_controller->loop();
    }
}


class Brightness__ControllerOff_Twinkle_TransformOff__BrightenedPixels: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_controller->on = false;
        test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        test_controller->twinkle = true;
        test_controller->transform = false;

        test_controller->brightness = 50;
        test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller->pixels[i].brightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOff_Twinkle_TransformOff__BrightenedPixels, brightness_down) {
    assertEqual(test_controller->brightness, 50);
    for (byte brightness = 255; brightness > 0; brightness--) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller->pixels[i].brightness, brightness);
        }
        test_controller->loop();
    }
}


class Brightness__ControllerOff_TwinkleOff_TransformOff__BrightenedPixels: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_controller->on = false;
        test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        test_controller->twinkle = false;
        test_controller->transform = false;

        test_controller->brightness = 50;
        test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller->pixels[i].brightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOff_TwinkleOff_TransformOff__BrightenedPixels, brightness_down) {
    assertEqual(test_controller->brightness, 50);
    for (byte brightness = 255; brightness > 0; brightness--) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller->pixels[i].brightness, brightness);
        }
        test_controller->loop();
    }
}

/* END brightness adjusts downward */

//////////////////////////////////////////////////////////////////////////////////////////////////////


/* START brightness adjusts upward */


class Brightness__ControllerOn_Twinkle_Transform__DimmedPixels: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_controller->on = true;
        test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        test_controller->twinkle = true;
        test_controller->transform = true;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller->pixels[i].targetBrightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOn_Twinkle_Transform__DimmedPixels, brightness_up) {
    assertEqual(test_controller->brightness, 255);
    assertFalse(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);

    for (byte brightness = 0; brightness < 255; brightness++) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller->pixels[i].brightness, brightness);
        }
        test_controller->loop();
    }
}


class Brightness__ControllerOn_Twinkle_TransformOff__DimmedPixels: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_controller->on = true;
        test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        test_controller->twinkle = true;
        test_controller->transform = false;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller->pixels[i].targetBrightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOn_Twinkle_TransformOff__DimmedPixels, brightness_up) {
    assertEqual(test_controller->brightness, 255);
    assertFalse(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);

    for (byte brightness = 0; brightness < 255; brightness++) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller->pixels[i].brightness, brightness);
        }
        test_controller->loop();
    }
}


class Brightness__ControllerOn_TwinkleOff_Transform__DimmedPixels: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_controller->on = true;
        test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        test_controller->twinkle = false;
        test_controller->transform = true;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller->pixels[i].setTargetBrightness(255);
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOn_TwinkleOff_Transform__DimmedPixels, brightness_up) {
    assertEqual(test_controller->brightness, 255);
    assertFalse(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);

    for (byte brightness = 0; brightness < 255; brightness++) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller->pixels[i].brightness, brightness);
        }
        test_controller->loop();
    }
}


class Brightness__ControllerOn_TwinkleOff_TransformOff__DimmedPixels: public aunit::TestOnce {
protected:
    void setup() override {
        aunit::TestOnce::setup();
        test_controller = new NeoPixelController;
        test_controller->init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_controller->on = true;
        test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        test_controller->twinkle = false;
        test_controller->transform = false;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller->pixels[i].targetBrightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOn_TwinkleOff_TransformOff__DimmedPixels, brightness_up) {
    assertEqual(test_controller->brightness, 255);
    assertFalse(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);

    for (byte brightness = 0; brightness < 255; brightness++) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller->pixels[i].brightness, brightness);
        }
        test_controller->loop();
    }
}

/* END brightness adjusts upward */

////////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {
    Serial.begin(115200);
#if defined(EPOXY_DUINO)
    Serial.setLineModeUnix();
#endif
}


void loop() {
    aunit::TestRunner::run();
}
