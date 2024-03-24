#include "connect_funcs.h"

#include "mqtt_funcs.h"

void setup() {
  Serial.begin(115200);
  Serial.print("\nInit ");
  Serial.print(CONTROLLER_TYPE);
  Serial.println(" ESP /w CTRLR ID: ");
  Serial.print(CONTROLLER_ID);
  Serial.println(" & CTRLR NAME: " + CONTROLLER_NAME);

  randomSeed(analogRead(0));
  pinMode(LED_BUILTIN, OUTPUT);
  setup_wifi();
  mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt_client.setCallback(handleMqttMsgReceived);
  // pinMode(ON_OR_OFF_PIN, OUTPUT);
}

void loop() {
  if (!mqtt_client.connected())
    connect_mqtt_broker(mqtt_client, publishEspStateOnOrOff);
  mqtt_client.loop();
}
