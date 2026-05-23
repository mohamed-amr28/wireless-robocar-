#ifndef ULTRASONIC_H
#define ULTRASONIC_H
 
#include <Arduino.h>

#define TRIG_FRONT 13
#define ECHO_FRONT 12

#define TRIG_LEFT  8
#define ECHO_LEFT  7

#define TRIG_RIGHT 11
#define ECHO_RIGHT 10

// USE EXTERN HERE! No equals signs here.
extern long duration;
extern float distance;
extern float frontDistance;
extern float leftDistance;
extern float rightDistance;

// FIXED: Added the third parameter (float previousValue) to match the .cpp file
float readDistance(int trigPin, int echoPin, float previousValue);
void setupSensors();
void updateallsensors();

#endif