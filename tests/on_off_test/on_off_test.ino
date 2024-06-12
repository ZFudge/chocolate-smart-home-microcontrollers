#line 2 "onofftest.ino"

#include <AUnitVerbose.h>
#include <chocolate-smart-home-microcontrollers.h>


class OnState: public aunit::TestOnce {
    protected:
    void setup() override {
        aunit::TestOnce::setup();
        OnOff::on_off_controller.on = true;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};


class OffState: public aunit::TestOnce {
    protected:
    void setup() override {
        aunit::TestOnce::setup();
        OnOff::on_off_controller.on = false;
    }

    void teardown() override {
        aunit::TestOnce::teardown();
    }
};


testF(OnState, turn_off_redundant) {
    for (byte i = 0; i < 3; i++) {
        OnOff::on_off_controller.turnOnOrOff(false);
        assertFalse(OnOff::on_off_controller.on);
    }
}


testF(OffState, turn_on_redundant) {
    for (byte i = 0; i < 3; i++) {
        OnOff::on_off_controller.turnOnOrOff(true);
        assertTrue(OnOff::on_off_controller.on);
    }
}


testF(OnState, getControllerState) {
    const String currentState = OnOff::getControllerState();
    assertEqual(currentState, "1");
}


testF(OffState, getControllerState) {
    const String currentState = OnOff::getControllerState();
    assertEqual(currentState, "0");
}


testF(OnState, processOnOffMsg_turn_off) {
    OnOff::processOnOffMsg("&on=0");
    assertFalse(OnOff::on_off_controller.on);
}


testF(OffState, processOnOffMsg_turn_on) {
    OnOff::processOnOffMsg("&on=1");
    assertTrue(OnOff::on_off_controller.on);
}


testF(OnState, processOnOffMsg_turn_off_msg_repeated) {
    OnOff::processOnOffMsg("&on=0&on=0");
    assertFalse(OnOff::on_off_controller.on);
}


testF(OffState, processOnOffMsg_turn_on_msg_repeated) {
    OnOff::processOnOffMsg("&on=1&on=1");
    assertTrue(OnOff::on_off_controller.on);
}


testF(OnState, processOnOffMsg_guard_clauses_on) {
    OnOff::processOnOffMsg("&on=0&on=1");
    assertTrue(OnOff::on_off_controller.on);
    OnOff::processOnOffMsg("&on=1&on=0");
    assertTrue(OnOff::on_off_controller.on);
}


testF(OffState, processOnOffMsg_guard_clauses_off) {
    OnOff::processOnOffMsg("&on=0&on=1");
    assertFalse(OnOff::on_off_controller.on);
    OnOff::processOnOffMsg("&on=1&on=0");
    assertFalse(OnOff::on_off_controller.on);
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
