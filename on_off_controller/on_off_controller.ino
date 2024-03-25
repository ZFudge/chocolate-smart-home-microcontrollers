#include "configuration.h"
#include "consts.h"
#include "data_funcs.h"

void setup() {
  Serial.begin(115200);

  controller.init(CONTROLLER_TYPE, MQTT_ID, NAME, getStateMessage);

  setup_wifi();

  init_mqtt_broker(handleMqttMsgReceivedOnOff, MQTT_SERVER, MQTT_PORT);

  pinMode(LED_BUILTIN, OUTPUT);
  // pinMode(ON_OR_OFF_PIN, OUTPUT);
}

void loop() {
  if (!mqtt_client.connected()) connect_mqtt_broker();
  mqtt_client.loop();
}
