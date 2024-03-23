#ifndef CONNECT_H_
#define CONNECT_H_

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "consts.h"
#include "state_values.h"
#include "turn_on_or_off.h"

WiFiClient espClient;
PubSubClient client(espClient);

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
    client.publish(SEND_THIS_CONTROLLERS_DATA_TOPIC, String(message).c_str())
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

void setup_wifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    flash();
    delay(500);
    Serial.print(".");
  }
  for (byte i = 0; i < 5; i++) {
    flash();
    delay(250 - (i * 50));
  }
  Serial.println();
  Serial.print("Connected to WiFi! Address: ");
  Serial.println(WiFi.localIP());
}

void connect_broker() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection on port ");
    Serial.print(MQTT_PORT);
    Serial.println("...");
    digitalWrite(LED_BUILTIN, LOW);
    if (client.connect(CONTROLLER_NAME.c_str())) {
      Serial.println("Connected!");
      Serial.print("Subscribing to ");
      Serial.print(NEW_DATA_RECEIVED_TOPIC + ", ");
      Serial.print(THIS_CONTROLLERS_STATE_REQUESTED_TOPIC + ", and ");
      Serial.println(ALL_CONTROLLERS_STATE_REQUESTED_TOPIC);
      for (byte i = 0; i < 5; i++) {
        flash();
        delay(i * 100);
      }
      client.subscribe(NEW_DATA_RECEIVED_TOPIC.c_str());
      client.subscribe(THIS_CONTROLLERS_STATE_REQUESTED_TOPIC.c_str());
      client.subscribe(ALL_CONTROLLERS_STATE_REQUESTED_TOPIC);
      Serial.println("Subscribed!");
      Serial.println("Attempting initial publish...");
      publishEspStateOnOrOff();
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in a few seconds...");
      flash();
      digitalWrite(LED_BUILTIN, HIGH);
      // Timeout before trying connection again.
      delay(7500);
    }
  }
}

#endif
