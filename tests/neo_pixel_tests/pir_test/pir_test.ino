#line 2 "pir_test.ino"

#include <AUnitVerbose.h>

#include <chocolate-smart-home-microcontrollers.h>


using namespace NeoPixel;


#define TEST_DATA_PIN   6
#define TEST_NUM_PIX    10

#define TEST_PIR_PIN    5


bool mockActivePinF(const byte pin) {
    return true;
}

test(pir_null) {
    /*controller.pir should be null if not enabled.*/
    NeoPixelController test_controller;
    assertEqual(test_controller.pir, NULL);
}

test(cannot_enable) {
    /*controller.pir should be null if trying to enable after initializing controller.*/
    NeoPixelController test_controller;
    test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);
    test_controller.enable_pir(TEST_PIR_PIN);
    assertEqual(test_controller.pir, NULL);
}


class PIR: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.enable_pir(TEST_PIR_PIN);
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);
    }
};

testF(PIR, controller_isOn) {
    // on by default
    assertTrue(test_controller.isOn());
    test_controller.on = false;
    assertFalse(test_controller.isOn());
    test_controller.pir->setTimeoutInSeconds(1);
    test_controller.pir->getReading = mockActivePinF;
    assertTrue(test_controller.isOn());
    delay(1000);
    // Revert mock reading so that motion is no longer detected.
    test_controller.pir->getReading = getReadingFromPin;
    assertFalse(test_controller.isOn());
}

testF(PIR, motionDetected) {
    test_controller.pir->setTimeoutInSeconds(1);
    test_controller.pir->getReading = mockActivePinF;
    assertTrue(test_controller.pir->motionDetected());
    // Revert mock reading so that motion is no longer detected.
    test_controller.pir->getReading = getReadingFromPin;
    delay(100);
    assertTrue(test_controller.pir->motionDetected());
    delay(100);
    assertTrue(test_controller.pir->motionDetected());
    delay(800);
    // Timeout window defined by pir->timeoutInSeconds has expired. Motion
    // should no longer be detected.
    assertFalse(test_controller.pir->motionDetected());
}

testF(PIR, setTimeoutInSeconds) {
    test_controller.pir->setTimeoutInSeconds(10);
    assertEqual(test_controller.pir->timeoutInSeconds, 10);
    test_controller.pir->setTimeoutInSeconds(77);
    assertEqual(test_controller.pir->timeoutInSeconds, 77);
    test_controller.pir->setTimeoutInSeconds(123);
    assertEqual(test_controller.pir->timeoutInSeconds, 123);
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
