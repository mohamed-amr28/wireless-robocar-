#ifndef AUTO_H
#define AUTO_H

#include <Arduino.h>



const int auto_speed =125;
const int autoturn_speed =175;
const int SAFE_DISTANCE = 25;   //cm
const int TURN_TIME = 600;      // wait some time while turning
const int SCAN_DELAY = 300;     // wait some time until find other path

// MUST use extern here so it shares the speed with bluetooth!
extern float currentSpeed;      

void runAutonomousMode();



#endif