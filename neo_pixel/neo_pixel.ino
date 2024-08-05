#include <Chocolate_Smart_Home_ESP_Connect.h>

#include "configuration.h"
#include "neo_pixel.h"


#define DATA_PIN   5
#define NUM_PIX    50


NeoPixel::NeoPixelController np_controller;


String getControllerState() {
    return DuplexMessenger::getNeoPixelControllerState(&np_controller);
}

void processMsg(String message) {
    DuplexMessenger::processNeoPixelMsg(message, &np_controller);
}


void setup() {
    Serial.begin(115200);
    np_controller.init(DATA_PIN, NUM_PIX, NEO_GRB + NEO_KHZ800);
    np_controller.setMS(1);

    // Use controller-specific methods for instructing controller state.
    CsmEspConn::controller.setGetState(getControllerState);
    CsmEspConn::controller.setProcessMsgReceived(processMsg);

    // Configure controller.
    CsmEspConn::controller.setMQTTId(MQTT_ID);
    CsmEspConn::controller.setType(CONTROLLER_TYPE);
    CsmEspConn::controller.init(NAME);

    CsmEspConn::connect_WIFI();

    CsmEspConn::set_mqtt_server_host_and_port(MQTT_SERVER, MQTT_PORT);

}


void loop() {
    if (!CsmEspConn::mqtt_client.connected())
        CsmEspConn::connect_MQTT();
    CsmEspConn::mqtt_client.loop();
    np_controller.loop();
}
