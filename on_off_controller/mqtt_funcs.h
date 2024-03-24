#ifndef MQTT_FUNCS_H_
#define MQTT_FUNCS_H_

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "consts.h"
#include "controller_data.h"
#include "state_values.h"
#include "turn_on_or_off.h"

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

ControllerData controller;


void publishEspState() {
  const String message = controller.getStateMessage();

  Serial.print("Publishing: ");
  Serial.println(message);
  Serial.print("To topic: ");
  Serial.println(SEND_THIS_CONTROLLERS_DATA_TOPIC);
  Serial.println(
    mqtt_client.publish(
      SEND_THIS_CONTROLLERS_DATA_TOPIC,
      String(message).c_str()
    )
    ? "Success!" : "FAILED!"
  );
}

void handleMqttMsgReceived(String topic, byte* message, unsigned int length) {
  /* This is called whenever a message is published to
   * a topic that the ESP8266 is subscribed to. */
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);

  if (topic == controller.stateRequestedTopic ||
      topic == ALL_CONTROLLERS_STATES_REQUESTED_TOPIC)
    return publishEspState();

  String messageTemp;
  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
    Serial.print((char)message[i]);
  }
  Serial.println();
  Serial.println(messageTemp);

  const bool on = messageTemp.indexOf("&on=1") != -1;
  const bool off = messageTemp.indexOf("&on=0") != -1;
  if (on && !off) {
    Serial.print("ON");
    turnOnOrOff(true);
  } else if (off && !on) {
    Serial.print("OFF");
    turnOnOrOff(false);
  }
  publishEspState();
}

void init_mqtt_broker() {
  mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt_client.setCallback(handleMqttMsgReceived);
}

#endif
