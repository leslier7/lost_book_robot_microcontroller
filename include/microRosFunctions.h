// Robbie Leslie
// April 2026

#ifndef MICROROS_FUNCTIONS_H
#define MICROROS_FUNCTIONS_H

#if defined(ROS) || defined(ROS_DEBUG)

bool microRosSetup();
bool create_entities();
void destroy_entities();
void microRosTick();

#endif

#endif // MICROROS_FUNCTIONS_H
