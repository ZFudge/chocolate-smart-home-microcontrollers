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


class PIR_1Sec: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    PIRReader test_pir;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_pir.init(TEST_PIR_PIN);
        bindNPCToPIR(&test_controller, &test_pir);
        test_pir.setTimeoutInSeconds(1);
    }
};

testF(PIR_1Sec, disarm) {
    assertTrue(test_pir.armed);
    test_pir.arm(0);
    assertFalse(test_pir.armed);
}

testF(PIR_1Sec, arm) {
    test_pir.armed = false;
    test_pir.arm(1);
    assertTrue(test_pir.armed);
}

testF(PIR_1Sec, setTimeoutInSeconds) {
    test_pir.setTimeoutInSeconds(10);
    assertEqual(test_pir.timeoutInSeconds, 10);
    test_pir.setTimeoutInSeconds(77);
    assertEqual(test_pir.timeoutInSeconds, 77);
    test_pir.setTimeoutInSeconds(123);
    assertEqual(test_pir.timeoutInSeconds, 123);
}

testF(PIR_1Sec, motionDetected) {
    test_pir.getReading = mockActivePinF;
    assertTrue(test_pir.motionDetected());
    // Revert mock reading so that motion is no longer detected.
    test_pir.getReading = getReadingFromPin;
    delay(100);
    assertTrue(test_pir.motionDetected());
    delay(100);
    assertTrue(test_pir.motionDetected());
    delay(800);
    // Timeout window defined by pir.timeoutInSeconds has expired. Motion
    // should no longer be detected.
    assertFalse(test_pir.motionDetected());
}

testF(PIR_1Sec, motionDetected_disarmed) {
    test_pir.getReading = mockActivePinF;
    assertTrue(test_pir.motionDetected());
    test_pir.arm(0);
    assertFalse(test_pir.motionDetected());
    // Revert mock reading so that motion is no longer detected.
    test_pir.getReading = getReadingFromPin;
    test_pir.arm(1);
    delay(100);
    test_pir.arm(0);
    assertFalse(test_pir.motionDetected());
    delay(100);
    test_pir.arm(1);
    assertTrue(test_pir.motionDetected());
    delay(800);
    // Timeout window defined by pir.timeoutInSeconds has expired. Motion
    // should no longer be detected.
    assertFalse(test_pir.motionDetected());
}

testF(PIR_1Sec, controller_isOn_false__off_motion_not_detected) {
    /*isOn() should return false when both controller.on and pir.motionDetected() are false*/
    test_controller.on = false;
    assertFalse(test_controller.on);
    assertFalse(test_pir.motionDetected());
    assertFalse(test_controller.isOn());
}

testF(PIR_1Sec, controller_isOn_true__off_motion_detected) {
    /*isOn() should return false when both controller.on and pir.motionDetected() are false*/
    test_controller.on = false;
    test_pir.getReading = mockActivePinF;
    assertTrue(test_pir.motionDetected());
    assertTrue(test_controller.isOn());

    /*isOn should return false after timeout*/
    delay(1000);
    // Revert mock reading so that motion is no longer detected.
    test_pir.getReading = getReadingFromPin;
    assertFalse(test_pir.motionDetected());
    assertFalse(test_controller.isOn());
}

testF(PIR_1Sec, controller_isOn_true__on_motion_not_detected) {
    /*isOn() should return true when controller.on is true and pir.motionDetected() is false*/
    assertTrue(test_controller.on);
    assertFalse(test_pir.motionDetected());
    assertTrue(test_controller.isOn());
}

testF(PIR_1Sec, controller_isOn_true__on_and_motion_detected) {
    /*isOn() should return true when both controller.on and pir.motionDetected() are true*/
    test_pir.getReading = mockActivePinF;
    assertTrue(test_controller.on);
    assertTrue(test_pir.motionDetected());
    assertTrue(test_controller.isOn());

    /*isOn should continue returning true after timeout*/
    delay(1000);
    // Revert mock reading so that motion is no longer detected.
    test_pir.getReading = getReadingFromPin;
    assertFalse(test_pir.motionDetected());
    assertTrue(test_controller.isOn());
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
