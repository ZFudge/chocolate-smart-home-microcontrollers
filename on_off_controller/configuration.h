#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

const uint8_t CONTROLLER_ID = 123;
const String controllerName = "Test";

const char* MQTT_SERVER = "";
const char* ssid = "";
const char* password = "";

#define DEFAULT_MQTT_PORT 1883
#define MQTT_PORT 1883

// Appending a randomly generated integer to the controller name
// helps the server determine when a reboot has occurred.
const String CONTROLLER_NAME = controllerName + " - " + String(random(0, 999));

#endif
