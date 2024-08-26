#line 2 "pir_on_off_test.ino"

#include <AUnitVerbose.h>

#include <chocolate-smart-home-microcontrollers.h>


using namespace NeoPixel;


#define TEST_DATA_PIN   6
#define TEST_NUM_PIX    10

#define TEST_PIR_PIN    5


bool mockActivePinF(const byte pin) {
    return true;
}


class PIR_transform_no_twinkle: public aunit::TestOnce {
protected:
    NeoPixelController *test_controller;
    PIRReader *test_pir;
    void setup() override {
        aunit::TestOnce::setup();
        test_pir = new PIRReader(TEST_PIR_PIN);
        test_controller = new NeoPixelController(test_pir);
        test_controller->init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_pir->setTimeoutInSeconds(1);
        test_controller->setTwinkle(0);
        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller->pixels[i].targetBrightness = 255;
    }
};

testF(PIR_transform_no_twinkle, on_off) {
    /*pir should trigger on/off transitions as expected when twinkle is off*/

    assertFalse(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    // Brighten up
    for (byte i = 0; i < 255; i++)
        test_controller->loop();
    assertTrue(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    // Colors
    for (byte i = 0; i < TEST_NUM_PIX; i++) {
        assertTrue(test_controller->pixels[i].brightness == 255);
        assertTrue(0 < test_controller->strip.getPixelColor(8));
    }
    // Turn off
    test_controller->turnOnOff(0);
    assertFalse(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    // Dim down
    for (int i = 0; i < 1000; i++)
        test_controller->loop();
    assertTrue(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    for (byte i = 0; i < TEST_NUM_PIX; i++) {
        assertTrue(test_controller->pixels[i].brightness == 0);
        assertTrue(0 == test_controller->strip.getPixelColor(8));
    }
    
    assertTrue(0 == test_pir->lastMotionDetected);
    // Trigger on using PIR sensor
    test_pir->getReading = mockActivePinF;
    test_controller->loop();
    assertTrue(test_controller->isOn());
    assertFalse(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    assertTrue(0 < test_pir->lastMotionDetected);
    for (byte i = 0; i < TEST_NUM_PIX; i++) {
        assertEqual(test_controller->pixels[i].brightness, 1);
        assertTrue(0 < test_controller->strip.getPixelColor(8));
    }
    // Brighten up
    for (byte expBrightness = 2; expBrightness < 255; expBrightness++) {
        test_controller->loop();

        assertTrue(test_controller->isOn());
        assertFalse(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
        assertTrue(0 < test_pir->lastMotionDetected);

        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller->pixels[i].brightness, expBrightness);
            assertTrue(0 < test_controller->strip.getPixelColor(8));
        }
    }
    test_controller->loop();
    for (byte i = 0; i < TEST_NUM_PIX; i++)
        assertEqual(test_controller->pixels[i].brightness, 255);
    test_pir->getReading = getReadingFromPin;
    test_controller->loop();
    assertTrue(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    // Wait for PIR timeout to expire.
    delay(2000);
    // Stop reading active pin.
    test_pir->getReading = getReadingFromPin;
    
    const int secondsLapsedSinceLastReading = (millis() - test_pir->lastMotionDetected) / 1000;
    assertMore(secondsLapsedSinceLastReading, test_pir->timeoutInSeconds);

    // Dim down
    for (byte expBrightness = 254; expBrightness > 1; expBrightness--) {
        test_controller->loop();

        assertFalse(test_controller->isOn());
        assertFalse(test_controller->on);
        assertFalse(test_controller->pirIsOn());
        assertFalse(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller->pixels[i].brightness, expBrightness);
            assertTrue(0 < test_controller->strip.getPixelColor(8));
        }
    }
}


class PIR_transform_twinkle: public aunit::TestOnce {
protected:
    NeoPixelController *test_controller;
    PIRReader *test_pir;
    void setup() override {
        aunit::TestOnce::setup();
        test_pir = new PIRReader(TEST_PIR_PIN);
        test_controller = new NeoPixelController(test_pir);
        test_controller->init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_pir->setTimeoutInSeconds(1);
        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller->pixels[i].targetBrightness = 255;
    }
};

testF(PIR_transform_twinkle, on_off) {
    /*pir should trigger on/off transitions as expected*/

    assertFalse(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    // Brighten up
    for (byte i = 0; i < 255; i++)
        test_controller->loop();
    assertTrue(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    // Colors
    for (byte i = 0; i < TEST_NUM_PIX; i++) {
        assertTrue(test_controller->pixels[i].brightness == 255);
        assertTrue(0 < test_controller->strip.getPixelColor(8));
    }
    // Turn off
    test_controller->turnOnOff(0);
    assertFalse(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    // Dim down
    for (int i = 0; i < 1000; i++)
        test_controller->loop();
    assertTrue(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    for (byte i = 0; i < TEST_NUM_PIX; i++) {
        assertTrue(test_controller->pixels[i].brightness == 0);
        assertTrue(0 == test_controller->strip.getPixelColor(8));
    }
    
    assertTrue(0 == test_pir->lastMotionDetected);
    // Trigger on using PIR sensor
    test_pir->getReading = mockActivePinF;
    test_controller->loop();
    assertTrue(test_controller->isOn());
    assertFalse(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    assertTrue(0 < test_pir->lastMotionDetected);
    for (byte i = 0; i < TEST_NUM_PIX; i++) {
        assertEqual(test_controller->pixels[i].brightness, 1);
        assertTrue(0 < test_controller->strip.getPixelColor(8));
    }
    // Brighten up
    for (byte expBrightness = 2; expBrightness < 255; expBrightness++) {
        test_controller->loop();

        assertTrue(test_controller->isOn());
        assertFalse(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
        assertTrue(0 < test_pir->lastMotionDetected);

        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller->pixels[i].brightness, expBrightness);
            assertTrue(0 < test_controller->strip.getPixelColor(8));
        }
    }
    test_controller->loop();
    for (byte i = 0; i < TEST_NUM_PIX; i++)
        assertEqual(test_controller->pixels[i].brightness, 255);
    test_pir->getReading = getReadingFromPin;
    test_controller->loop();
    assertTrue(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    // Wait for PIR timeout to expire
    delay(2000);
    // Stop reading active pin.
    test_pir->getReading = getReadingFromPin;
    
    const int secondsLapsedSinceLastReading = (millis() - test_pir->lastMotionDetected) / 1000;
    assertMore(secondsLapsedSinceLastReading, test_pir->timeoutInSeconds);

    // Dim down
    for (byte expBrightness = 253; expBrightness > 1; expBrightness--) {
        test_controller->loop();

        assertFalse(test_controller->isOn());
        assertFalse(test_controller->on);
        assertFalse(test_controller->pirIsOn());
        assertFalse(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller->pixels[i].brightness, expBrightness);
            assertTrue(0 < test_controller->strip.getPixelColor(8));
        }
    }
}


class PIR_no_transform_no_twinkle: public aunit::TestOnce {
protected:
    NeoPixelController *test_controller;
    PIRReader *test_pir;
    void setup() override {
        aunit::TestOnce::setup();
        test_pir = new PIRReader(TEST_PIR_PIN);
        test_controller = new NeoPixelController(test_pir);
        test_controller->init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_controller->setTwinkle(0);
        test_controller->setTransform(0);
        test_pir->setTimeoutInSeconds(1);
        for (byte i = 0; i < TEST_NUM_PIX; i++)
            test_controller->pixels[i].targetBrightness = 255;
    }
};

testF(PIR_no_transform_no_twinkle, on_off) {
    /*pir should trigger on/off transitions as expected when transform and twinkle are both off*/

    assertFalse(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    // Brighten up
    for (byte i = 0; i < 255; i++)
        test_controller->loop();
    assertTrue(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    // Colors
    for (byte i = 0; i < TEST_NUM_PIX; i++) {
        assertTrue(test_controller->pixels[i].brightness == 255);
        assertTrue(0 < test_controller->strip.getPixelColor(8));
    }
    // Turn off
    test_controller->turnOnOff(0);
    assertFalse(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    // Dim down
    for (int i = 0; i < 1000; i++)
        test_controller->loop();
    assertTrue(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    for (byte i = 0; i < TEST_NUM_PIX; i++) {
        assertTrue(test_controller->pixels[i].brightness == 0);
        assertTrue(0 == test_controller->strip.getPixelColor(8));
    }
    
    assertTrue(0 == test_pir->lastMotionDetected);
    // Trigger on using PIR sensor
    test_pir->getReading = mockActivePinF;
    test_controller->loop();
    assertTrue(test_controller->isOn());
    assertFalse(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    assertTrue(0 < test_pir->lastMotionDetected);
    for (byte i = 0; i < TEST_NUM_PIX; i++) {
        assertEqual(test_controller->pixels[i].brightness, 1);
        assertTrue(0 < test_controller->strip.getPixelColor(8));
    }
    // Brighten up
    for (byte expBrightness = 2; expBrightness < 255; expBrightness++) {
        test_controller->loop();

        assertTrue(test_controller->isOn());
        assertFalse(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
        assertTrue(0 < test_pir->lastMotionDetected);

        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller->pixels[i].brightness, expBrightness);
            assertTrue(0 < test_controller->strip.getPixelColor(8));
        }
    }
    test_controller->loop();
    for (byte i = 0; i < TEST_NUM_PIX; i++)
        assertEqual(test_controller->pixels[i].brightness, 255);
    test_pir->getReading = getReadingFromPin;
    test_controller->loop();
    assertTrue(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
    // Wait for PIR timeout to expire
    delay(2000);
    // Stop reading active pin.
    test_pir->getReading = getReadingFromPin;
    
    const int secondsLapsedSinceLastReading = (millis() - test_pir->lastMotionDetected) / 1000;
    assertMore(secondsLapsedSinceLastReading, test_pir->timeoutInSeconds);

    // Dim down
    for (byte expBrightness = 254; expBrightness > 1; expBrightness--) {
        test_controller->loop();

        assertFalse(test_controller->isOn());
        assertFalse(test_controller->on);
        assertFalse(test_controller->pirIsOn());
        assertFalse(test_controller->ALL_PIXELS_BRIGHTNESS_ARE_CURRENT);
        for (byte i = 0; i < TEST_NUM_PIX; i++) {
            assertEqual(test_controller->pixels[i].brightness, expBrightness);
            assertTrue(0 < test_controller->strip.getPixelColor(8));
        }
    }
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
