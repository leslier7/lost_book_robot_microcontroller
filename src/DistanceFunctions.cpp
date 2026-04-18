// Robbie Leslie
// April 2026

#include "DistanceFunctions.h"
#include <Arduino.h>
#include <Wire.h>
#include <VL53L1X.h>

VL53L1X dist_sensors[5];


void pcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(PCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

// bool setupTof(){
//     Wire.begin();

//     Serial.println("\nPCAScanner ready!");
//     int num_sensors = 0;
    
//     for (uint8_t t=0; t<8; t++) {
//       pcaselect(t);
//       Serial.print("PCA Port #"); Serial.println(t);

//       for (uint8_t addr = 0; addr<=127; addr++) {
//         if (addr == PCAADDR) continue;

//         Wire.beginTransmission(addr);
//         if (!Wire.endTransmission()) {
//           Serial.print("Found I2C 0x");  Serial.println(addr,HEX);
//           num_sensors++;
//         }
//       }
//     }
//     if(num_sensors != 5){
//         Serial.println("Didn't find all 5 sensors");
//         return false;
//     } else {

//       for (int i = 0 ; i < 5; i++){
//         pcaselect(i);
//         bool started = dist_sensors[i].init();

//         delay(5);

//         if(!started){
//           Serial.printf("Sensor %d failed to start\n", i);
//           return false;
//         }

//         dist_sensors[i].startContinuous(50);
//       }

//     }
//     return true;
// }

// Probably a better version
bool setupTof(){
    Wire.begin();

    for (int i = 0; i < 5; i++) {
        pcaselect(i);

        Wire.beginTransmission(0x29);
        if (Wire.endTransmission() != 0) {
            Serial.printf("No VL53L1X found on port %d\n", i);
            return false;
        }

        if (!dist_sensors[i].init()) {
            Serial.printf("Sensor %d failed to init\n", i);
            return false;
        }
        dist_sensors[i].startContinuous(50);
    }
    return true;
}

int getSensor(DistSensor_t sensor){

  switch (sensor){
    case FRONT:
      break;

    case LEFT:
      break;
    
    case RIGHT:
      break;

    case BACK:
      break;

    case EXTRA:
      break;

    default:
      break;
  }


  return 0;
}

