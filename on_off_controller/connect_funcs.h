#ifndef CONNECT_H_
#define CONNECT_H_

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "consts.h"

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

void connect_mqtt_broker(PubSubClient& client, void (*publish)()) {
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
      publish();
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
