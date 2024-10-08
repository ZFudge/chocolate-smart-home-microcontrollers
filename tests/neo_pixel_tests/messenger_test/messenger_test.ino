#line 2 "messenger_test.ino"

#include <AUnitVerbose.h>

#include <chocolate-smart-home-microcontrollers.h>


using namespace NeoPixel;


#define TEST_DATA_PIN   6
#define TEST_NUM_PIX    10

#define TEST_PIR_PIN    5


class ControllerProcessMsg: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    PIRReader test_pir;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_pir.init(TEST_PIR_PIN);
        bindNPCToPIR(&test_controller, &test_pir);
    }
};

// getByteValueUsingKey start
testF(ControllerProcessMsg, getByteValueUsingKey_on_true) {
    const String incomingMessage = "on=1;";
    assertTrue((bool)Utils::getByteValueUsingKey(incomingMessage, "on"));
}

testF(ControllerProcessMsg, getByteValueUsingKey_on_false) {
    const String incomingMessage = "on=0;";
    assertFalse((bool)Utils::getByteValueUsingKey(incomingMessage, "on"));
}

testF(ControllerProcessMsg, getByteValueUsingKey_ms) {
    const String incomingMessage = "ms=4;";
    const byte expectedResult = 4;
    const byte result = Utils::getByteValueUsingKey(incomingMessage, "ms");
    assertEqual(result, expectedResult);
}

testF(ControllerProcessMsg, getByteValueUsingKey_brightness) {
    const String incomingMessage = "brightness=123;";
    const byte expectedResult = 123;
    const byte result = Utils::getByteValueUsingKey(incomingMessage, "brightness");
    assertEqual(result, expectedResult);
}

testF(ControllerProcessMsg, getByteValueUsingKey_twinkle_true) {
    const String incomingMessage = "twinkle=1;";
    assertTrue((bool)Utils::getByteValueUsingKey(incomingMessage, "twinkle"));
}

testF(ControllerProcessMsg, getByteValueUsingKey_twinkle_false) {
    const String incomingMessage = "twinkle=0;";
    assertFalse((bool)Utils::getByteValueUsingKey(incomingMessage, "twinkle"));
}

testF(ControllerProcessMsg, getByteValueUsingKey_transform_true) {
    const String incomingMessage = "transform=1;";
    assertTrue((bool)Utils::getByteValueUsingKey(incomingMessage, "transform"));
}

testF(ControllerProcessMsg, getByteValueUsingKey_transform_false) {
    const String incomingMessage = "transform=0;";
    assertFalse((bool)Utils::getByteValueUsingKey(incomingMessage, "transform"));
}
// getByteValueUsingKey end

testF(ControllerProcessMsg, processNeoPixelMsg_on) {
    /*processNeoPixelMsg should use the incoming message to set controller.on to true.*/
    const String incomingMessage = "on=1;";
    DuplexMessenger::processNeoPixelMsg(incomingMessage, &test_controller);
    assertTrue(test_controller.on);
}

testF(ControllerProcessMsg, processNeoPixelMsg_off) {
    /*processNeoPixelMsg should use the incoming message to set controller.on to false.*/
    const String incomingMessage = "on=0;";
    DuplexMessenger::processNeoPixelMsg(incomingMessage, &test_controller);
    assertFalse(test_controller.on);
}

testF(ControllerProcessMsg, processNeoPixelMsg_ms) {
    /*processNeoPixelMsg should use the incoming message to set controller.ms to 5.*/
    const String incomingMessage = "ms=5;";
    DuplexMessenger::processNeoPixelMsg(incomingMessage, &test_controller);
    assertEqual(test_controller.ms, 5);
}

testF(ControllerProcessMsg, processNeoPixelMsg_brightness) {
    /*processNeoPixelMsg should use the incoming message to set controller.brightness to 123.*/
    const String incomingMessage = "brightness=123;";
    DuplexMessenger::processNeoPixelMsg(incomingMessage, &test_controller);
    assertEqual(test_controller.brightness, 123);
}

testF(ControllerProcessMsg, processNeoPixelMsg_twinkle_true) {
    /*processNeoPixelMsg should use the incoming message to set controller.twinkle to true.*/
    const String incomingMessage = "twinkle=1;";
    DuplexMessenger::processNeoPixelMsg(incomingMessage, &test_controller);
    assertTrue(test_controller.twinkle);
}

testF(ControllerProcessMsg, processNeoPixelMsg_twinkle_false) {
    /*processNeoPixelMsg should use the incoming message to set controller.twinkle to false.*/
    const String incomingMessage = "twinkle=0;";
    DuplexMessenger::processNeoPixelMsg(incomingMessage, &test_controller);
    assertFalse(test_controller.twinkle);
}

testF(ControllerProcessMsg, processNeoPixelMsg_transform_true) {
    /*processNeoPixelMsg should use the incoming message to set controller.transform to true.*/
    const String incomingMessage = "transform=1;";
    DuplexMessenger::processNeoPixelMsg(incomingMessage, &test_controller);
    assertTrue(test_controller.transform);
}

testF(ControllerProcessMsg, processNeoPixelMsg_transform_false) {
    /*processNeoPixelMsg should use the incoming message to set controller.transform to false.*/
    const String incomingMessage = "transform=0;";
    DuplexMessenger::processNeoPixelMsg(incomingMessage, &test_controller);
    assertFalse(test_controller.transform);
}

testF(ControllerProcessMsg, processNeoPixelMsg_palette) {
    const String incomingMessage =
        "palette=0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,255;";
    DuplexMessenger::processNeoPixelMsg(incomingMessage, &test_controller);
    assertEqual(rgbs[0][0], 0);
    assertEqual(rgbs[0][1], 10);
    assertEqual(rgbs[0][2], 20);
    assertEqual(rgbs[1][0], 30);
    assertEqual(rgbs[1][1], 40);
    assertEqual(rgbs[1][2], 50);
    assertEqual(rgbs[2][0], 60);
    assertEqual(rgbs[2][1], 70);
    assertEqual(rgbs[2][2], 80);
    assertEqual(rgbs[3][0], 90);
    assertEqual(rgbs[3][1], 100);
    assertEqual(rgbs[3][2], 110);
    assertEqual(rgbs[4][0], 120);
    assertEqual(rgbs[4][1], 130);
    assertEqual(rgbs[4][2], 140);
    assertEqual(rgbs[5][0], 150);
    assertEqual(rgbs[5][1], 160);
    assertEqual(rgbs[5][2], 170);
    assertEqual(rgbs[6][0], 180);
    assertEqual(rgbs[6][1], 190);
    assertEqual(rgbs[6][2], 200);
    assertEqual(rgbs[7][0], 210);
    assertEqual(rgbs[7][1], 220);
    assertEqual(rgbs[7][2], 230);
    assertEqual(rgbs[8][0], 240);
    assertEqual(rgbs[8][1], 250);
    assertEqual(rgbs[8][2], 255);

}


testF(ControllerProcessMsg, processNeoPixelMsg_all) {
    const String incomingMessage =
        (String)"on=0;" +
        (String)"brightness=234;" +
        (String)"ms=7;" +
        (String)"twinkle=0;" +
        (String)"transform=0;" +
        (String)"palette=0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,255;";

    assertTrue(test_controller.on);
    assertEqual(test_controller.brightness, 255);
    assertEqual(test_controller.ms, 0);
    assertTrue(test_controller.twinkle);
    assertTrue(test_controller.transform);

    DuplexMessenger::processNeoPixelMsg(incomingMessage, &test_controller);

    assertFalse(test_controller.on);
    assertEqual(test_controller.brightness, 234);
    assertEqual(test_controller.ms, 7);
    assertFalse(test_controller.twinkle);
    assertFalse(test_controller.transform);
    assertEqual(rgbs[0][0], 0);
    assertEqual(rgbs[0][1], 10);
    assertEqual(rgbs[0][2], 20);
    assertEqual(rgbs[1][0], 30);
    assertEqual(rgbs[1][1], 40);
    assertEqual(rgbs[1][2], 50);
    assertEqual(rgbs[2][0], 60);
    assertEqual(rgbs[2][1], 70);
    assertEqual(rgbs[2][2], 80);
    assertEqual(rgbs[3][0], 90);
    assertEqual(rgbs[3][1], 100);
    assertEqual(rgbs[3][2], 110);
    assertEqual(rgbs[4][0], 120);
    assertEqual(rgbs[4][1], 130);
    assertEqual(rgbs[4][2], 140);
    assertEqual(rgbs[5][0], 150);
    assertEqual(rgbs[5][1], 160);
    assertEqual(rgbs[5][2], 170);
    assertEqual(rgbs[6][0], 180);
    assertEqual(rgbs[6][1], 190);
    assertEqual(rgbs[6][2], 200);
    assertEqual(rgbs[7][0], 210);
    assertEqual(rgbs[7][1], 220);
    assertEqual(rgbs[7][2], 230);
    assertEqual(rgbs[8][0], 240);
    assertEqual(rgbs[8][1], 250);
    assertEqual(rgbs[8][2], 255);

}


class ControllerRGBs: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
    PIRReader test_pir;
    void setup() override {
        aunit::TestOnce::setup();
        test_controller.init(TEST_DATA_PIN, TEST_NUM_PIX);
        test_pir.init(TEST_PIR_PIN);
        bindNPCToPIR(&test_controller, &test_pir);
        test_controller.setMS(4);
        rgbs[0][0] = 9;
        rgbs[0][1] = 10;
        rgbs[0][2] = 20;
        rgbs[1][0] = 30;
        rgbs[1][1] = 40;
        rgbs[1][2] = 50;
        rgbs[2][0] = 60;
        rgbs[2][1] = 70;
        rgbs[2][2] = 80;
        rgbs[3][0] = 90;
        rgbs[3][1] = 100;
        rgbs[3][2] = 110;
        rgbs[4][0] = 120;
        rgbs[4][1] = 130;
        rgbs[4][2] = 140;
        rgbs[5][0] = 150;
        rgbs[5][1] = 160;
        rgbs[5][2] = 170;
        rgbs[6][0] = 180;
        rgbs[6][1] = 190;
        rgbs[6][2] = 200;
        rgbs[7][0] = 210;
        rgbs[7][1] = 220;
        rgbs[7][2] = 230;
        rgbs[8][0] = 240;
        rgbs[8][1] = 250;
        rgbs[8][2] = 255;
    }
};

testF(ControllerRGBs, getNeoPixelControllerState) {
    /* getNeoPixelControllerState should return a string
    representation of the controller's state with the current palette appended. */

    /* Ascending bit order
       controller->on
       controller->twinkle
       controller->transform
       controller->allTwinkleColorsAreCurrent
       controller->pir
       controller->pir->timeoutInSeconds */
    const String boolByte = "63";
    const String ms = "4";
    const String brightness = "255";
    const String pirTimeout = "150";
    const String palette = "9,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,255";
    const String expectedResult =
        boolByte + "," +
        ms + "," +
        brightness + "," +
        pirTimeout + "," +
        palette;
    const String result = DuplexMessenger::getNeoPixelControllerState(&test_controller);
    assertStringCaseEqual(result, expectedResult);
}

testF(ControllerRGBs, getNeoPixelControllerState__pir_timeoutInSeconds) {
    /*getNeoPixelControllerState should return a string that includes the
    pir.timeoutInSeconds value at the expected position. */

    test_controller.pir->setTimeoutInSeconds(98);
    const String boolByte = "63,";
    const String ms = "4,";
    const String brightness = "255,";
    const String pirTimeout = "98,";
    const String expectedResult = boolByte + ms + brightness + pirTimeout;
    const String result = DuplexMessenger::getNeoPixelControllerState(&test_controller);
    assertStringCaseEqual(result.substring(0, 12), expectedResult);
}

testF(ControllerRGBs, getNeoPixelControllerState__no_pir) {
    /*getNeoPixelControllerState should return a byte value without the pir and pir.armed bits set.*/
    test_controller.pir = NULL;
    /* Ascending bit order
    x  controller->on
    x  controller->twinkle
    x  controller->transform
    x  controller->allTwinkleColorsAreCurrent
       controller->pir
       controller->pir->armed */
    const String result = DuplexMessenger::getNeoPixelControllerState(&test_controller);
    assertStringCaseEqual(result.substring(0, 2), "15");
}

testF(ControllerRGBs, getNeoPixelControllerState__pir_armed) {
    /*getNeoPixelControllerState should return a byte value with the pir bit set and pir.armed bit not set.*/
    test_controller.pir->arm(0);
    /* Ascending bit order
    x  controller->on
    x  controller->twinkle
    x  controller->transform
    x  controller->allTwinkleColorsAreCurrent
    x  controller->pir
       controller->pir->armed */
    const String boolByte = "31";
    const String result = DuplexMessenger::getNeoPixelControllerState(&test_controller);
    assertStringCaseEqual(result.substring(0, 2), boolByte);
}

testF(ControllerRGBs, getNeoPixelControllerState__allTwinkleColorsAreCurrent_1_color_mismatch) {
    /*getNeoPixelControllerState should return a byte value with the allTwinkleColorsAreCurrent
    bit set, despite the first pixel's color not matching its colorIndex color.*/
    test_controller.pixels[0].r = 0;
    /* Ascending bit order
       controller->on
       controller->twinkle
       controller->transform
       controller->allTwinkleColorsAreCurrent
       controller->pir
       controller->pir->timeoutInSeconds */
    const String boolByte = "63";
    const String result = DuplexMessenger::getNeoPixelControllerState(&test_controller);
    assertStringCaseEqual(result.substring(0, 2), boolByte);
}

testF(ControllerRGBs, getNeoPixelControllerState__allTwinkleColorsAreCurrent_1_color_mismatch_no_transform) {
    /*getNeoPixelControllerState should return a byte value with the allTwinkleColorsAreCurrent bit
    set when transform is off, even though the first pixel's color does not match its colorIndex color.*/
    test_controller.transform = false;
    test_controller.pixels[0].r = 0;
    /* Ascending bit order
    x  controller->on
    x  controller->twinkle
       controller->transform
    x  controller->allTwinkleColorsAreCurrent
    x  controller->pir
    x  controller->pir->armed */
    const String boolByte = "51";
    const String result = DuplexMessenger::getNeoPixelControllerState(&test_controller);
    assertStringCaseEqual(result.substring(0, 2), boolByte);
}

testF(ControllerRGBs, getNeoPixelControllerState__allTwinkleColorsAreCurrent_1_color_mismatch_no_twinkle) {
    /*getNeoPixelControllerState should return a byte value with the allTwinkleColorsAreCurrent bit not set when twinkle is off.*/
    test_controller.twinkle = false;
    // First pixel color does not match its colorIndex color to demonstrate that color is ignored.
    test_controller.pixels[0].r = 0;
    /* Ascending bit order
    x  controller->on
       controller->twinkle
    x  controller->transform
    x  controller->allTwinkleColorsAreCurrent
    x  controller->pir
    x  controller->pir->timeoutInSeconds */
    const String boolByte = "61";
    const String result = DuplexMessenger::getNeoPixelControllerState(&test_controller);
    assertStringCaseEqual(result.substring(0, 2), boolByte);
}

testF(ControllerRGBs, getNeoPixelControllerState__allTwinkleColorsAreCurrent_1_color_mismatch_no_transform_no_twinkle) {
    /*getNeoPixelControllerState should return a byte value with the allTwinkleColorsAreCurrent bit set when transform and twinkle are off.*/
    test_controller.transform = false;
    test_controller.twinkle = false;
    // First pixel color does not match its colorIndex color to demonstrate that color is ignored.
    test_controller.pixels[0].r = 0;
    /* Ascending bit order
    x  controller->on
       controller->twinkle
       controller->transform
    x  controller->allTwinkleColorsAreCurrent
    x  controller->pir
    x  controller->pir->armed */
    const String boolByte = "57";
    const String result = DuplexMessenger::getNeoPixelControllerState(&test_controller);
    assertStringCaseEqual(result.substring(0, 2), boolByte);
}


class RGBs: public aunit::TestOnce {
protected:
    NeoPixelController test_controller;
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
        rgbs[3][0] = 90;
        rgbs[3][1] = 100;
        rgbs[3][2] = 110;
        rgbs[4][0] = 120;
        rgbs[4][1] = 130;
        rgbs[4][2] = 140;
        rgbs[5][0] = 150;
        rgbs[5][1] = 160;
        rgbs[5][2] = 170;
        rgbs[6][0] = 180;
        rgbs[6][1] = 190;
        rgbs[6][2] = 200;
        rgbs[7][0] = 210;
        rgbs[7][1] = 220;
        rgbs[7][2] = 230;
        rgbs[8][0] = 240;
        rgbs[8][1] = 250;
        rgbs[8][2] = 255;
    }
};

testF(RGBs, getCsvRGBs) {
    /*getCsvRGBs should returned the current color palette as the expected string.*/
    const String expectedResult = "0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,255";
    assertStringCaseEqual(getCsvRGBs(), expectedResult);
}

testF(ControllerProcessMsg, pir_armed_true) {
    /*processNeoPixelMsg should use the incoming message to set controller.pir.armed to true.*/
    const String incomingMessage = "pir_armed=1;";
    DuplexMessenger::processNeoPixelMsg(incomingMessage, &test_controller);
    assertTrue(test_controller.pir->armed);
}

testF(ControllerProcessMsg, pir_armed_false) {
    /*processNeoPixelMsg should use the incoming message to set controller.pir.armed to false.*/
    const String incomingMessage = "pir_armed=0;";
    DuplexMessenger::processNeoPixelMsg(incomingMessage, &test_controller);
    assertFalse(test_controller.pir->armed);
}

testF(ControllerProcessMsg, pir_timeout) {
    /*processNeoPixelMsg should use the incoming message to set controller.pir.timeoutInSeconds value to 255, and subsequently, 123, 10, and 1.*/
    String incomingMessage = "pir_timeout=255;";
    DuplexMessenger::processNeoPixelMsg(incomingMessage, &test_controller);
    assertEqual(test_controller.pir->timeoutInSeconds, 255);

    incomingMessage = "pir_timeout=123;";
    DuplexMessenger::processNeoPixelMsg(incomingMessage, &test_controller);
    assertEqual(test_controller.pir->timeoutInSeconds, 123);

    incomingMessage = "pir_timeout=10;";
    DuplexMessenger::processNeoPixelMsg(incomingMessage, &test_controller);
    assertEqual(test_controller.pir->timeoutInSeconds, 10);

    incomingMessage = "pir_timeout=1;";
    DuplexMessenger::processNeoPixelMsg(incomingMessage, &test_controller);
    assertEqual(test_controller.pir->timeoutInSeconds, 1);
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
