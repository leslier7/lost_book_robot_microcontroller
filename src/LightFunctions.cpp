#include <Arduino.h>
#include "LightFunctions.h"

bool light_status = false;

void setupLight(){
    analogWriteFreq(LIGHT_PWM_FREQ);
    pinMode(LIGHT_PIN, OUTPUT);
}

void setLightBrightness(float brightness){
    analogWrite(LIGHT_PIN, (uint8_t)(brightness * 255.0f));
    light_status = brightness > 0.0f;
}

void setLight(bool value){
    setLightBrightness(value ? 1.0f : 0.0f);
}

void turnOnLight(){
    setLightBrightness(1.0f);
}

void turnOffLight(){
    setLightBrightness(0.0f);
}

bool getLightValue(){
    return light_status;
}