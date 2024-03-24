#ifndef CONSTS_H_
#define CONSTS_H_

#include "configuration.h"

void flash() {
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
}

const char CONTROLLER_TYPE[] = "ON_OR_OFF";

const char ALL_CONTROLLERS_STATES_REQUESTED_TOPIC[]  = "/request_devices_state/";
const char SEND_THIS_CONTROLLERS_DATA_TOPIC[]        = "/receive_device_state/";

const uint8_t D0   = 16;
const uint8_t D1   = 5;
const uint8_t D2   = 4;
const uint8_t D3   = 0;
const uint8_t D4   = 2;
const uint8_t D5   = 14;
const uint8_t D6   = 12;
const uint8_t D7   = 13;
const uint8_t D8   = 15;

const uint8_t ON_OR_OFF_PIN = 8;

#endif
