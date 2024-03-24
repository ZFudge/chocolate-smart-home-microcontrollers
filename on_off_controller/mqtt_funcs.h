#ifndef MQTT_FUNCS_H_
#define MQTT_FUNCS_H_

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "consts.h"
#include "state_values.h"
#include "turn_on_or_off.h"

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

void publishEspStateOnOrOff() {
  String message = (
    String(CONTROLLER_TYPE) + ',' +
    String(CONTROLLER_ID) + ',' +
    String(CONTROLLER_NAME) + ',' +
    String(onOrOff)
  );

  Serial.print("Publishing: ");
  Serial.println(message);
  Serial.print("To topic: ");
  Serial.println(SEND_THIS_CONTROLLERS_DATA_TOPIC);
  Serial.println(
    mqtt_client.publish(SEND_THIS_CONTROLLERS_DATA_TOPIC, String(message).c_str())
    ? "Success!" : "FAILED!"
  );
}

void handleMqttMsgReceived(String topic, byte* message, unsigned int length) {
  /* This is called whenever a message is published to
   * a topic that the ESP8266 is subscribed to. */
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);

  if (topic == THIS_CONTROLLERS_STATE_REQUESTED_TOPIC ||
      topic == ALL_CONTROLLERS_STATE_REQUESTED_TOPIC)
    return publishEspStateOnOrOff();

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
  publishEspStateOnOrOff();
}

#endif
