#include "configuration.h"
#include "consts.h"
#include "data_funcs.h"


void setup() {
  Serial.begin(115200);
  Serial.print("\nInit ");
  Serial.print(CONTROLLER_TYPE);
  Serial.println(" ESP /w CTRLR ID: ");
  Serial.print(MQTT_ID);
  Serial.println(" & CTRLR NAME: " + controller.name);

  randomSeed(analogRead(0));
  pinMode(LED_BUILTIN, OUTPUT);
  controller.init(CONTROLLER_TYPE, MQTT_ID, NAME, getStateMessage);
  setup_wifi();
  init_mqtt_broker(handleMqttMsgReceivedOnOff, MQTT_SERVER, MQTT_PORT);
  // pinMode(ON_OR_OFF_PIN, OUTPUT);
}

void loop() {
  if (!mqtt_client.connected()) connect_mqtt_broker(MQTT_PORT, MQTT_SERVER);
  mqtt_client.loop();
}
