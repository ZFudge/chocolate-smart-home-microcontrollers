#include "connect_funcs.h"

#include "mqtt_funcs.h"

const uint8_t MQTT_ID = 123;
const String  NAME = "Test";

String getStateMessage() {
  return (
    String(CONTROLLER_TYPE) + ',' +
    String(MQTT_ID) + ',' +
    String(NAME) + ',' +
    String(onOrOff)
  );
}

void setup() {
  Serial.begin(115200);
  Serial.print("\nInit ");
  Serial.print(CONTROLLER_TYPE);
  Serial.println(" ESP /w CTRLR ID: ");
  Serial.print(MQTT_ID);
  Serial.println(" & CTRLR NAME: " + NAME);

  randomSeed(analogRead(0));
  pinMode(LED_BUILTIN, OUTPUT);
  controller.init(CONTROLLER_TYPE, MQTT_ID, NAME, getStateMessage);
  setup_wifi();
  init_mqtt_broker();
  // pinMode(ON_OR_OFF_PIN, OUTPUT);
}

void loop() {
  if (!mqtt_client.connected()) connect_mqtt_broker();
  mqtt_client.loop();
}
