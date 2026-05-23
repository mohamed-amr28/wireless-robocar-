#ifndef AUTOPARK1_H
#define AUTOPARK1_H

#include <Arduino.h>

// --- MOTOR SPEEDS ---
#define SCAN_SPEED          140  
#define TURN_SPEED          140   
#define correction          100
// --- NEW SENSOR THRESHOLDS ---
#define FRONT_MIN_CM        10.0f // Min safety distance for parking
#define SLOT_DETECT_THRESH  40.0f // Max distance to detect a "wall"

// --- UPDATED PARKING MATRIX ---
#define SLOT_MIN_TIME_MS    200UL 
#define OFFSET_MS           350UL  
#define TURN1_MS            900UL  
#define STRAIGHT_MS         1000UL  
#define TURN2_MS            900UL 
#define CORRECT_MS          200UL

// --- STATE MACHINE ENUMS ---
typedef enum {
    PARK_IDLE,
    PARK_SCAN_LOOK_FOR_WALL,
    PARK_SCAN_PASSING_WALL,
    PARK_SCAN_MEASURING_GAP,
    PARK_POSITIONING,
    PARK_MANEUVER_TURN1,
    PARK_MANEUVER_STRAIGHT,
    PARK_MANEUVER_TURN2,
    PARK_MANEUVER_CORRECT,
    PARK_DONE,
    PARK_FAILED
} ParkState;

typedef enum {
     PARK_RIGHT,
    PARK_LEFT,
} ParkSide;

// --- GLOBAL SHARED VARIABLES ---
extern ParkState currentParkState;
extern bool isParkingModeActive;

// --- CORE FRAMEWORK APIs ---
void initAutoPark(void);
void startAutoPark(void);
void stopAutoPark(void);
void handleAutoParkingMode(void);

#endif // AUTOPARK1_H