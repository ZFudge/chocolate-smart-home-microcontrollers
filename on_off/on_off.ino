#include <Chocolate_Smart_Home_ESP_Connect.h>

#include "on_off.h"


void setup() {
    Serial.begin(115200);

    CsmEspConn::controller.setGetState(getControllerState);
    CsmEspConn::controller.setProcessMsgReceived(processOnOffMsg);
    
    CsmEspConn::controller.setMQTTId(MQTT_ID);
    CsmEspConn::controller.setType(CONTROLLER_TYPE);
    CsmEspConn::controller.init(NAME);

    CsmEspConn::connect_WIFI();

    CsmEspConn::set_mqtt_server_host_and_port(MQTT_SERVER, MQTT_PORT);

    pinMode(LED_BUILTIN, OUTPUT);
    // pinMode(ON_OFF_PIN, OUTPUT);
}


void loop() {
    if (!CsmEspConn::mqtt_client.connected())
        CsmEspConn::connect_MQTT();
    CsmEspConn::mqtt_client.loop();
}
