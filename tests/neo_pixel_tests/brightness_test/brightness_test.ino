#line 2 "brightness_test.ino"

#include <AUnitVerbose.h>

#include <chocolate-smart-home-microcontrollers.h>


using namespace NeoPixel;


#define TEST_DATA_PIN   6
#define TEST_NUM_PIX    10


/* Test that pixel.brightness is automatically adjusted to the correct level,
 * or range, when the controller.brightness setting is changed.
 */


/* START brightness adjusts downward */

class Brightness__ControllerOn__Twinkle__Transform__BrightenedPixels: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);

        test_controller.twinkle = true;
        test_controller.transform = true;

        test_controller.brightness = 50;
        test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller.pixels[i].brightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOn__Twinkle__Transform__BrightenedPixels, brightness_down) {
    /* pixel.brightness should decrement on each controller.loop when pixel.brightness > controller.brightness
    Controller  on
    Twinkle     on
    Transform   on */
    for (byte i = 0; i < TEST_NUM_PIX; i++)
        assertEqual(test_controller.pixels[i].targetBrightness, 0);
    assertEqual(test_controller.brightness, 50);

    for (byte expectedBrightness = 255; expectedBrightness >= 50; expectedBrightness--) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller.pixels[i].brightness, expectedBrightness);
        }
        test_controller.loop();
    }
}


class Brightness__ControllerOn__Twinkle__TransformOff__BrightenedPixels: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);

        test_controller.twinkle = true;
        test_controller.transform = false;

        test_controller.brightness = 50;
        test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller.pixels[i].brightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOn__Twinkle__TransformOff__BrightenedPixels, brightness_down) {
    /* pixel.brightness should decrement on each controller.loop when pixel.brightness > controller.brightness
    Controller  on
    Twinkle     on
    Transform   off */
    for (byte i = 0; i < TEST_NUM_PIX; i++)
        assertEqual(test_controller.pixels[i].targetBrightness, 0);
    assertEqual(test_controller.brightness, 50);

    for (byte expectedBrightness = 255; expectedBrightness >= 50; expectedBrightness--) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller.pixels[i].brightness, expectedBrightness);
        }
        test_controller.loop();
    }
}


class Brightness__ControllerOn__TwinkleOff__Transform__BrightenedPixels: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);

        test_controller.twinkle = false;
        test_controller.transform = true;

        test_controller.brightness = 50;
        test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller.pixels[i].brightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOn__TwinkleOff__Transform__BrightenedPixels, brightness_down) {
    /* pixel.brightness should decrement on each controller.loop when pixel.brightness > controller.brightness
    Controller  on
    Twinkle     off
    Transform   on */
    for (byte i = 0; i < TEST_NUM_PIX; i++)
        assertEqual(test_controller.pixels[i].targetBrightness, 0);
    assertEqual(test_controller.brightness, 50);

    for (byte expectedBrightness = 255; expectedBrightness >= 50; expectedBrightness--) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller.pixels[i].brightness, expectedBrightness);
        }
        test_controller.loop();
    }
}


class Brightness__ControllerOn__TwinkleOff__TransformOff__BrightenedPixels: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);

        test_controller.twinkle = false;
        test_controller.transform = false;

        test_controller.brightness = 50;
        test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller.pixels[i].brightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOn__TwinkleOff__TransformOff__BrightenedPixels, brightness_down) {
    /* pixel.brightness should decrement on each controller.loop when pixel.brightness > controller.brightness
    Controller  on
    Twinkle     off
    Transform   off */
    for (byte i = 0; i < TEST_NUM_PIX; i++)
        assertEqual(test_controller.pixels[i].targetBrightness, 0);
    assertEqual(test_controller.brightness, 50);

    for (byte expectedBrightness = 255; expectedBrightness >= 50; expectedBrightness--) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller.pixels[i].brightness, expectedBrightness);
        }
        test_controller.loop();
    }
}


class Brightness__ControlleOff_Twinkle_Transform__BrightenedPixels: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_controller.on = false;

        test_controller.twinkle = true;
        test_controller.transform = true;

        test_controller.brightness = 50;
        test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller.pixels[i].brightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControlleOff_Twinkle_Transform__BrightenedPixels, brightness_down) {
    /* pixel.brightness should decrement on each controller.loop when pixel.brightness > controller.brightness
    Controller  off
    Twinkle     on
    Transform   on */
    for (byte i = 0; i < TEST_NUM_PIX; i++)
        assertEqual(test_controller.pixels[i].targetBrightness, 0);
    assertEqual(test_controller.brightness, 50);
    assertFalse(test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);

    for (byte expectedBrightness = 255; expectedBrightness > 0; expectedBrightness--) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller.pixels[i].brightness, expectedBrightness);
        }
        test_controller.loop();
    }
}


class Brightness__ControllerOff_TwinkleOff_Transform__BrightenedPixels: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_controller.on = false;

        test_controller.twinkle = false;
        test_controller.transform = true;

        test_controller.brightness = 50;
        test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller.pixels[i].brightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOff_TwinkleOff_Transform__BrightenedPixels, brightness_down) {
    /* pixel.brightness should decrement on each controller.loop when
    pixel.brightness > controller.brightness, until
    ALL_PIXELS_BRIGHTNESS_ARE_CURRENT is true, even when controller is off.
    Controller  off
    Twinkle     off
    Transform   on */
    assertEqual(test_controller.brightness, 50);
    assertFalse(test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    for (byte i = 0; i < TEST_NUM_PIX; i++)
        assertEqual(test_controller.pixels[i].targetBrightness, 0);

    for (byte expectedBrightness = 255; expectedBrightness > 0; expectedBrightness--) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller.pixels[i].brightness, expectedBrightness);
        }
        test_controller.loop();
    }
}


class Brightness__ControllerOff_Twinkle_TransformOff__BrightenedPixels: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_controller.on = false;
        test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        test_controller.twinkle = true;
        test_controller.transform = false;

        test_controller.brightness = 50;
        test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller.pixels[i].brightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOff_Twinkle_TransformOff__BrightenedPixels, brightness_down) {
    /* pixel.brightness should decrement on each controller.loop when
    pixel.brightness > controller.brightness, until
    ALL_PIXELS_BRIGHTNESS_ARE_CURRENT is true, even when controller is off.
    Controller  off
    Twinkle     on
    Transform   off */
    for (byte i = 0; i < TEST_NUM_PIX; i++)
        assertEqual(test_controller.pixels[i].targetBrightness, 0);
    assertEqual(test_controller.brightness, 50);
    assertFalse(test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);

    for (byte expectedBrightness = 255; expectedBrightness > 0; expectedBrightness--) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller.pixels[i].brightness, expectedBrightness);
        }
        test_controller.loop();
    }
}


class Brightness__ControllerOff_TwinkleOff_TransformOff__BrightenedPixels: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_controller.on = false;
        test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        test_controller.twinkle = false;
        test_controller.transform = false;

        test_controller.brightness = 50;
        test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller.pixels[i].brightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOff_TwinkleOff_TransformOff__BrightenedPixels, brightness_down) {
    /* pixel.brightness should decrement on each controller.loop when
    pixel.brightness > controller.brightness, until
    ALL_PIXELS_BRIGHTNESS_ARE_CURRENT is true, even when controller is off.
    Controller  off
    Twinkle     off
    Transform   off */
    for (byte i = 0; i < TEST_NUM_PIX; i++)
        assertEqual(test_controller.pixels[i].targetBrightness, 0);
    assertEqual(test_controller.brightness, 50);
    assertFalse(test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);

    for (byte expectedBrightness = 255; expectedBrightness > 0; expectedBrightness--) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller.pixels[i].brightness, expectedBrightness);
        }
        test_controller.loop();
    }
}

/* END brightness adjusts downward */

//////////////////////////////////////////////////////////////////////////////////////////////////////


/* START brightness adjusts upward */


class Brightness__ControllerOn_Twinkle_Transform__DimmedPixels: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_controller.on = true;
        test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        test_controller.twinkle = true;
        test_controller.transform = true;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller.pixels[i].targetBrightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOn_Twinkle_Transform__DimmedPixels, brightness_up) {
    /* pixel.brightness should increment towards its target brightness of 255, on each
    controller.loop, when target brightness is not greater than controller.brightness.
    Controller  on
    Twinkle     on
    Transform   on */
    assertEqual(test_controller.brightness, 255);
    assertFalse(test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);

    for (byte expectedBrightness = 0; expectedBrightness < 255; expectedBrightness++) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller.pixels[i].brightness, expectedBrightness);
        }
        test_controller.loop();
    }
}


class Brightness__ControllerOn_Twinkle_TransformOff__DimmedPixels: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_controller.on = true;
        test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        test_controller.twinkle = true;
        test_controller.transform = false;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller.pixels[i].targetBrightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOn_Twinkle_TransformOff__DimmedPixels, brightness_up) {
    /* pixel.brightness should increment towards its target brightness of 255, on each
    controller.loop, when target brightness is not greater than controller.brightness.
    Controller  on
    Twinkle     on
    Transform   off */
    assertEqual(test_controller.brightness, 255);
    assertFalse(test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);

    for (byte expectedBrightness = 0; expectedBrightness < 255; expectedBrightness++) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller.pixels[i].brightness, expectedBrightness);
        }
        test_controller.loop();
    }
}


class Brightness__ControllerOn_TwinkleOff_Transform__DimmedPixels: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_controller.on = true;
        test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        test_controller.twinkle = false;
        test_controller.transform = true;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller.pixels[i].setTargetBrightness(255);
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOn_TwinkleOff_Transform__DimmedPixels, brightness_up) {
    /* Pixel brightnesses should increment towards controller.brightnesses on
    each controller.loop.
    Controller  on
    Twinkle     off
    Transform   on */
    assertEqual(test_controller.brightness, 255);
    assertFalse(test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);

    for (byte expectedBrightness = 0; expectedBrightness < 255; expectedBrightness++) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller.pixels[i].brightness, expectedBrightness);
        }
        test_controller.loop();
    }
}


class Brightness__ControllerOn_TwinkleOff_TransformOff__DimmedPixels: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_controller.on = true;
        test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT = false;

        test_controller.twinkle = false;
        test_controller.transform = false;

        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller.pixels[i].targetBrightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(Brightness__ControllerOn_TwinkleOff_TransformOff__DimmedPixels, brightness_up) {
    /* Pixel brightnesses should increment towards controller.brightnesses on
    each controller.loop.
    Controller  on
    Twinkle     off
    Transform   off */
    assertEqual(test_controller.brightness, 255);
    assertFalse(test_controller.ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);

    for (byte expectedBrightness = 0; expectedBrightness < 255; expectedBrightness++) {
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller.pixels[i].brightness, expectedBrightness);
        }
        test_controller.loop();
    }
}

/* END brightness adjusts upward */

////////////////////////////////////////////////////////////////////////////////////////////////////


/* START controller.brightness determines resulting RGB values */

class RGBs_1: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    Pixel *pixel;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, 1);
        test_controller.brightness = 255;

        pixel = &test_controller.pixels[0];

        pixel->r = 237;
        pixel->g = 100;
        pixel->b = 55;
        pixel->brightness = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};


testF(RGBs_1, controller_brightness_full_rgbs) {
    /* rgb values should remain the same when controller brightness and pixel
    brightness are at max */
    test_controller.applyPixelSettingsToNeoPixel(0, pixel);
    const long pixelColor = test_controller.strip.getPixelColor(0);
    const byte r = pixelColor >> 16;
    const byte g = pixelColor >> 8 & 255;
    const byte b = pixelColor & 255;
    assertEqual(r, 237);
    assertEqual(g, 100);
    assertEqual(b, 55);
}

testF(RGBs_1, controller_brightness_dims_rgbs) {
    /* rgb values used in controller.strip should be the same intensity as
    controller.brightness, when pixel.brightness is at max */
    test_controller.brightness = 234;
    test_controller.applyPixelSettingsToNeoPixel(0, pixel);
    long pixelColor = test_controller.strip.getPixelColor(0);
    byte r = pixelColor >> 16;
    byte g = pixelColor >> 8 & 255;
    byte b = pixelColor & 255;
    assertEqual(r, 217);
    assertEqual(g, 92);
    assertEqual(b, 50);

    test_controller.brightness = 190;
    test_controller.applyPixelSettingsToNeoPixel(0, pixel);
    pixelColor = test_controller.strip.getPixelColor(0);
    r = pixelColor >> 16;
    g = pixelColor >> 8 & 255;
    b = pixelColor & 255;
    assertEqual(r, 177);
    assertEqual(g, 75);
    assertEqual(b, 41);

    test_controller.brightness = 167;
    test_controller.applyPixelSettingsToNeoPixel(0, pixel);
    pixelColor = test_controller.strip.getPixelColor(0);
    r = pixelColor >> 16;
    g = pixelColor >> 8 & 255;
    b = pixelColor & 255;
    assertEqual(r, 155);
    assertEqual(g, 65);
    assertEqual(b, 36);

    test_controller.brightness = 90;
    test_controller.applyPixelSettingsToNeoPixel(0, pixel);
    pixelColor = test_controller.strip.getPixelColor(0);
    r = pixelColor >> 16;
    g = pixelColor >> 8 & 255;
    b = pixelColor & 255;
    assertEqual(r, 84);
    assertEqual(g, 35);
    assertEqual(b, 19);

    test_controller.brightness = 12;
    test_controller.applyPixelSettingsToNeoPixel(0, pixel);
    pixelColor = test_controller.strip.getPixelColor(0);
    r = pixelColor >> 16;
    g = pixelColor >> 8 & 255;
    b = pixelColor & 255;
    assertEqual(r, 11);
    assertEqual(g, 5);
    assertEqual(b, 3);
}

testF(RGBs_1, controller_brightness_off_rgbs) {
    /* rgb values used in controller.strip are off or near off when
    controller.brightness is off or near off */
    test_controller.brightness = 0;
    test_controller.applyPixelSettingsToNeoPixel(0, pixel);
    long pixelColor = test_controller.strip.getPixelColor(0);
    byte r = pixelColor >> 16;
    byte g = pixelColor >> 8 & 255;
    byte b = pixelColor & 255;
    assertEqual(r, 0);
    assertEqual(g, 0);
    assertEqual(b, 0);

    test_controller.brightness = 1;
    test_controller.applyPixelSettingsToNeoPixel(0, pixel);
    pixelColor = test_controller.strip.getPixelColor(0);
    r = pixelColor >> 16;
    g = pixelColor >> 8 & 255;
    b = pixelColor & 255;
    assertEqual(r, 1);
    assertEqual(g, 0);
    assertEqual(b, 0);
}

/* END controller.brightness determines resulting RGB values */
/* START pixel.brightness determines resulting RGB values */

class RGBs_2: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    Pixel *pixel;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, 1);

        pixel = &test_controller.pixels[0];
        pixel->r = 5;
        pixel->g = 167;
        pixel->b = 255;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};

testF(RGBs_2, pixel_brightness_dims_rgbs) {
    /* rgb values used in controller.strip should be the same intensity as
    pixel.brightness, when controller.brightness is at max */
    pixel->brightness = 255;
    test_controller.applyPixelSettingsToNeoPixel(0, pixel);
    long pixelColor = test_controller.strip.getPixelColor(0);
    byte r = pixelColor >> 16;
    byte g = pixelColor >> 8 & 255;
    byte b = pixelColor & 255;
    assertEqual(r, 5);
    assertEqual(g, 167);
    assertEqual(b, 255);

    pixel->brightness = 190;
    test_controller.applyPixelSettingsToNeoPixel(0, pixel);
    pixelColor = test_controller.strip.getPixelColor(0);
    r = pixelColor >> 16;
    g = pixelColor >> 8 & 255;
    b = pixelColor & 255;
    assertEqual(r, 4);
    assertEqual(g, 124);
    assertEqual(b, 190);
}

testF(RGBs_2, pixel_brightness_off_rgbs) {
    /* rgb values used in controller.strip are off or near off
    when pixel.brightness is off or near off */
    pixel->brightness = 1;
    test_controller.applyPixelSettingsToNeoPixel(0, pixel);
    long pixelColor = test_controller.strip.getPixelColor(0);
    byte r = pixelColor >> 16;
    byte g = pixelColor >> 8 & 255;
    byte b = pixelColor & 255;
    assertEqual(r, 0);
    assertEqual(g, 1);
    assertEqual(b, 1);

    pixel->brightness = 0;
    test_controller.applyPixelSettingsToNeoPixel(0, pixel);
    pixelColor = test_controller.strip.getPixelColor(0);
    r = pixelColor >> 16;
    g = pixelColor >> 8 & 255;
    b = pixelColor & 255;
    assertEqual(r, 0);
    assertEqual(g, 0);
    assertEqual(b, 0);
}

/* END pixel.brightness determines resulting RGB values */
/* START controller.brightness and pixel.brightness determines resulting RGB values */

testF(RGBs_2, controller_and_pixel_brightness_full_rgbs) {
    /* rgb values should remain the same when controller brightness and pixel
    brightness are at max */
    pixel->brightness = 255;
    test_controller.brightness = 255;
    test_controller.applyPixelSettingsToNeoPixel(0, pixel);

    const long pixelColor = test_controller.strip.getPixelColor(0);
    const byte r = pixelColor >> 16;
    const byte g = pixelColor >> 8 & 255;
    const byte b = pixelColor & 255;
    assertEqual(r, 5);
    assertEqual(g, 167);
    assertEqual(b, 255);
}

testF(RGBs_2, controller_and_pixel_brightness_dims_rgbs) {
    /* rgb values used in controller.strip are the same intensity product of
    pixel.brightness and controller.brightness */
    pixel->brightness = 127;
    test_controller.brightness = 255;
    test_controller.applyPixelSettingsToNeoPixel(0, pixel);
    long pixelColor = test_controller.strip.getPixelColor(0);
    byte r = pixelColor >> 16;
    byte g = pixelColor >> 8 & 255;
    byte b = pixelColor & 255;
    assertEqual(r, 2);
    assertEqual(g, 83);
    assertEqual(b, 127);

    pixel->brightness = 255;
    test_controller.brightness = 127;
    test_controller.applyPixelSettingsToNeoPixel(0, pixel);
    pixelColor = test_controller.strip.getPixelColor(0);
    r = pixelColor >> 16;
    g = pixelColor >> 8 & 255;
    b = pixelColor & 255;
    assertEqual(r, 2);
    assertEqual(g, 83);
    assertEqual(b, 127);

    pixel->brightness = 127;
    test_controller.brightness = 127;
    test_controller.applyPixelSettingsToNeoPixel(0, pixel);
    pixelColor = test_controller.strip.getPixelColor(0);
    r = pixelColor >> 16;
    g = pixelColor >> 8 & 255;
    b = pixelColor & 255;
    assertEqual(r, 1);
    assertEqual(g, 41);
    assertEqual(b, 63);
}

testF(RGBs_2, controller_and_pixel_brightness_off_rgbs) {
    /* rgb values used in controller.strip are off when either
    controller.brightness or pixel.brightness is off */
    pixel->brightness = 0;
    test_controller.brightness = 255;
    test_controller.applyPixelSettingsToNeoPixel(0, pixel);
    long pixelColor = test_controller.strip.getPixelColor(0);
    byte r = pixelColor >> 16;
    byte g = pixelColor >> 8 & 255;
    byte b = pixelColor & 255;
    assertEqual(r, 0);
    assertEqual(g, 0);
    assertEqual(b, 0);

    pixel->brightness = 127;
    test_controller.brightness = 0;
    test_controller.applyPixelSettingsToNeoPixel(0, pixel);
    pixelColor = test_controller.strip.getPixelColor(0);
    r = pixelColor >> 16;
    g = pixelColor >> 8 & 255;
    b = pixelColor & 255;
    assertEqual(r, 0);
    assertEqual(g, 0);
    assertEqual(b, 0);

    pixel->brightness = 0;
    test_controller.brightness = 0;
    test_controller.applyPixelSettingsToNeoPixel(0, pixel);
    pixelColor = test_controller.strip.getPixelColor(0);
    r = pixelColor >> 16;
    g = pixelColor >> 8 & 255;
    b = pixelColor & 255;
    assertEqual(r, 0);
    assertEqual(g, 0);
    assertEqual(b, 0);
}

/* END controller.brightness determines resulting RGB values */

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
