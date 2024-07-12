#include "neo_pixel.h"


#define DATA_PIN   6
#define NUM_PIX    10


NeoPixel::NeoPixelController controller;


String getControllerState() {
    return DuplexMessenger::getNeoPixelControllerState(controller);
}

void processMsg(String message) {
    DuplexMessenger::processNeoPixelMsg(message, controller);
}


void setup() {
    Serial.begin(115200);
    controller.init(DATA_PIN, NUM_PIX);
    controller.setMS(5);
}


void loop() {
    controller.loop();
}
