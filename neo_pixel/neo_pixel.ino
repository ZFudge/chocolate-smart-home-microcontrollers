#include "neo_pixel.h"


#define DATA_PIN   6
#define NUM_PIX    10


NeoPixel::NeoPixelController controller;


void setup() {
    Serial.begin(115200);
    controller.init(DATA_PIN, NUM_PIX);
}


void loop() {
    controller.loop();
}
