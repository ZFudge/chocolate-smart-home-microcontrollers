#include "configuration.h"
#include "consts.h"
#include "duplex_messenger.h"


void setup() {
    Serial.begin(115200);

    controller_CSM.setGetControllerState(getControllerState);
    controller_CSM.setProcessMsgForController(processOnOffMsg);
    controller_CSM.setMQTTId(MQTT_ID);
    controller_CSM.setControllerType(CONTROLLER_TYPE);
    controller_CSM.init(NAME);

    connect_WIFI_CSM();

    set_mqtt_server_host_and_port_CSM(MQTT_SERVER, MQTT_PORT);

    pinMode(LED_BUILTIN, OUTPUT);
    // pinMode(ON_OFF_PIN, OUTPUT);
}


void loop() {
    if (!mqtt_client_CSM.connected()) connect_MQTT_CSM();
    mqtt_client_CSM.loop();
}
