#ifndef TURN_ON_OR_OFF_H_
#define TURN_ON_OR_OFF_H_

#include "consts.h"
#include "state_values.h"

void turnOnOrOff(bool newOnOrOff) {
  Serial.print(newOnOrOff ? "ON" : "OFF");
  if (newOnOrOff == onOrOff) return;
  onOrOff = newOnOrOff;
  digitalWrite(ON_OR_OFF_PIN, onOrOff);
}

#endif
