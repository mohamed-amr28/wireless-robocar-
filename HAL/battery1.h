#ifndef BATTERY_H

#define BATTERY_H

#include <Arduino.h>

const float batterydead =5.0;
const float batterymax = 12.0-batterydead;    
const float BATT_CRITICAL = 60.0;

const long updateInterval = 3000;

float getBatteryPercent();
void monitorBattery();

#endif