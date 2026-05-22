#include <Arduino.h>
#include <Wire.h>

#include "LightFunctions.h"
#include "DistanceFunctions.h"
#include "microRosFunctions.h"

bool light_val = false;

void setup() {
  Serial.begin(115200);
  unsigned long start = millis();
  while (!Serial && millis() - start < 10000) delay(10);

  setupLight();
  
  for(int i = 0; i < 3; i++){
    setLight(true);
    delay(500);
    setLight(false);
    delay(500);
  }

  
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

// light_val = !light_val;
// setLight(light_val);

}
