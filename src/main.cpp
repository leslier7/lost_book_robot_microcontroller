#include <Arduino.h>
#include <Wire.h>


#include "LightFunctions.h"
#include "DistanceFunctions.h"


void setup() {
  Serial.begin(115200);
  unsigned long start = millis();
  while (!Serial && millis() - start < 3000) delay(10);
  Serial.println("Hello world!\n");

  setupLight();
  setLight(true);
  setupTof();
}

void loop() {
  // delay(3000);
  // bool temp_light = !getLightValue();
  // setLight(temp_light);
}
