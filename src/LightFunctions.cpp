#include <Arduino.h>
#include "LightFunctions.h"

bool light_status = false;

void setupLight(){
    pinMode(LIGHT_PIN, OUTPUT);
}

void setLight(bool value){
    digitalWrite(LIGHT_PIN, value);
    light_status = value;
}

void turnOnLight(){
    digitalWrite(LIGHT_PIN, HIGH);
    light_status = true;
}

void turnOffLight(){
    digitalWrite(LIGHT_PIN, LOW);
    light_status = false;
}

bool getLightValue(){
    return light_status;
}