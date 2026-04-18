// Robbie Leslie
// April 2026

#ifndef DISTANCE_FUNCTIONS_H
#define DISTANCE_FUNCTIONS_H

#define PCAADDR 0x70

typedef enum {
    FRONT,
    LEFT,
    RIGHT,
    BACK,
    EXTRA
} DistSensor_t;

bool setupTof();

int getDistance(DistSensor_t sensor);

#endif