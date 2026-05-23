#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>

// Global mode variables shared across your project
extern char currentMode; 
extern float currentSpeed;

// --- YOUR NEW CLEAN FUNCTIONS ---
void Bluetooth_Init(void);
char Bluetooth_ReadCommand(void);
void Bluetooth_Send(const char* message);

// The core loop processing function
void handleBluetooth();
void sendSensorData();
#endif