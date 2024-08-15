#include <Chocolate_Smart_Home_ESP_Connect.h>

#include "configuration.h"
#include "neo_pixel.h"


#define DATA_PIN   5
#define NUM_PIX    50


#define LED_WIFI   19
#define LED_MQTT   18

#define PIR_PIN    15


NeoPixel::NeoPixelController np_controller;


String getControllerState() {
    return DuplexMessenger::getNeoPixelControllerState(&np_controller);
}

void processMsg(String message) {
    DuplexMessenger::processNeoPixelMsg(message, &np_controller);
}


void setup() {
    Serial.begin(115200);
    pinMode(LED_WIFI, OUTPUT);
    pinMode(LED_MQTT, OUTPUT);
    digitalWrite(LED_WIFI, LOW);
    digitalWrite(LED_MQTT, LOW);

    np_controller.enable_pir(PIR_PIN);
    // np_controller.setMaxCount(25);
    // np_controller.init(DATA_PIN, NUM_PIX, NEO_GRB + NEO_KHZ800);
    np_controller.init(DATA_PIN, NUM_PIX, NEO_RGB + NEO_KHZ800);
    np_controller.setMS(5);

    // Use controller-specific methods for instructing controller state.
    CsmEspConn::controller.setGetState(getControllerState);
    CsmEspConn::controller.setProcessMsgReceived(processMsg);

    // Configure controller.
    CsmEspConn::controller.setMQTTId(MQTT_ID);
    CsmEspConn::controller.setType(CONTROLLER_TYPE);
    CsmEspConn::controller.init(NAME);

    CsmEspConn::set_mqtt_server_host_and_port(MQTT_SERVER, MQTT_PORT);
}


void loop() {
    if (!CsmEspConn::wifiIsConnected())
        CsmEspConn::connect_WIFI(LED_WIFI);

    if (!CsmEspConn::mqtt_client.connected())
        CsmEspConn::connect_MQTT(LED_MQTT);
    else
        CsmEspConn::mqtt_client.loop();

    np_controller.loop();
}
