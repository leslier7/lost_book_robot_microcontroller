#include <Arduino.h>
#include <Wire.h>
#include <VL53L1X.h>

#include "LightFunctions.h"


void setup() {
  setupLight();
  setLight(true);
}

void loop() {
  delay(3000);
  bool temp_light = !getLightValue();
  setLight(temp_light);
}
