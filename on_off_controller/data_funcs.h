#ifndef DATA_FUNCS_H_
#define DATA_FUNCS_H_

#include "Chocolate_Smart_Home_ESP_Connect.h"

#include "configuration.h"
#include "consts.h"
#include "turn_on_or_off.h"

String getStateMessage() {
  return (String(CONTROLLER_TYPE) + ',' + String(MQTT_ID) + ',' +
          String(controller.name) + ',' + String(onOrOff));
}

void processNewData(String message) {
  const bool on = message.indexOf("&on=1") != -1;
  const bool off = message.indexOf("&on=0") != -1;

  if (on && !off) {
    turnOnOrOff(true);
  } else if (off && !on) {
    turnOnOrOff(false);
  }
}

void handleMqttMsgReceivedOnOff(String topic,
                                byte* message,
                                unsigned int length) {
  /* This is called whenever a message is published to
   * a topic that the ESP8266 is subscribed to. */
  handleMqttMsgReceived(topic, message, length, processNewData);
}

#endif
