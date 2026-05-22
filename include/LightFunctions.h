// Light Functions
// By Robbie Leslie

#define LIGHT_PIN 8
#define LIGHT_PWM_FREQ 50

void setupLight();

void setLight(bool value);

void setLightBrightness(float brightness); // 0.0 to 1.0

void turnOnLight();

void turnOffLight();

bool getLightValue();