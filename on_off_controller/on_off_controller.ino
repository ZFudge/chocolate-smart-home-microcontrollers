#include "connect_funcs.h"

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
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(handleMqttMsgReceived);
  // pinMode(ON_OR_OFF_PIN, OUTPUT);
}

void loop() {
  if (!client.connected()) connect_broker();
  client.loop();
}
