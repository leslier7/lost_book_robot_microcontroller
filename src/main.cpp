#include <Arduino.h>
#include <Wire.h>

#include "LightFunctions.h"
#include "DistanceFunctions.h"
#include "microRosFunctions.h"


void setup() {
  Serial.begin(115200);
  unsigned long start = millis();
  while (!Serial && millis() - start < 10000) delay(10);

  setupLight();
  setLight(true);
  setupTof();

#ifdef ROS
  microRosSetup();
#endif
}

void loop() {
#ifdef ROS
  microRosTick();
#else
  Serial.printf("FRONT: %d mm\r\n", getDistance(FRONT));
  Serial.printf("LEFT:  %d mm\r\n", getDistance(LEFT));
  Serial.printf("RIGHT: %d mm\r\n", getDistance(RIGHT));
  Serial.printf("BACK:  %d mm\r\n", getDistance(BACK));
  Serial.printf("EXTRA: %d mm\r\n", getDistance(EXTRA));
  Serial.print("\r\n");
  delay(500);
#endif
}
