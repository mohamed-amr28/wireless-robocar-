#include "autopark1.h"
#include "../HAL/ultrasonic1.h" 
#include "../HAL/motor1.h"      
#include "../HAL/bluetooth1.h"

// Instantiate public globals
ParkState currentParkState = PARK_IDLE;
bool isParkingModeActive = false;

// Internal logic tracking variables
static ParkSide park_side = PARK_RIGHT;
static unsigned long gap_start_time = 0;
static unsigned long maneuver_timer = 0;

// De-bounce validation counters (Filters out single-frame ultrasonic noise spikes)
static int wall_confirm_counter = 0;
static int gap_confirm_counter = 0;

// Missing variables added here
static float measured_side_distance = 15.0f;
static unsigned long dynamic_offset_ms = 0;
static unsigned long dynamic_turn1_ms = 0;
static unsigned long dynamic_straight_ms = 0;
static unsigned long dynamic_turn2_ms = 0;


void initAutoPark(void) {
    // Non-blocking initialization on boot
    isParkingModeActive = false;
    currentParkState = PARK_IDLE;
    wall_confirm_counter = 0;
    gap_confirm_counter = 0;
    gap_start_time = 0;
}

void startAutoPark(void) {
    isParkingModeActive = true;
    currentParkState = PARK_SCAN_LOOK_FOR_WALL;
    wall_confirm_counter = 0;
    gap_confirm_counter = 0;
    gap_start_time = 0;
    maneuver_timer = millis();
    Serial.println("\n=============================================");
    Serial.println("[AUTOPARK START]: Driving to find structural wall...");
    Serial.println("=============================================");
}

void stopAutoPark(void) {
    isParkingModeActive = false;
    currentParkState = PARK_IDLE;
    stopMotors();
}

void handleAutoParkingMode(void) {
    // 1. Constantly refresh all 3 hardware ultrasonic fields
    updateallsensors(); 

    // Lock out the state loop completely if already finished or failed
    if (currentParkState == PARK_DONE || currentParkState == PARK_FAILED) {
        stopMotors();
        return;
    }

    // Corrected logic flag condition to only arm when mode is NOT active yet
    if (!isParkingModeActive) {
        startAutoPark();
    }

    // 2. CRITICAL FRONT CRASH OVERRIDE
    if (frontDistance < FRONT_MIN_CM && frontDistance > 1.0f) {
        stopMotors();
        Serial.println("!!! EMERGENCY ABORT: Front crash trajectory blocked !!!");
        return;
    }

    unsigned long currentMillis = millis();
    // Logic updated to track the OPPOSITE side
    float active_side_distance = (park_side == PARK_LEFT) ? rightDistance : leftDistance;

    // 3. Complete Sequential Control Processing Engine
    switch (currentParkState) {

        case PARK_SCAN_LOOK_FOR_WALL:
            moveForward(SCAN_SPEED);
            
            // SWAPPED: Check LEFT side first for wall
            if (leftDistance < SLOT_DETECT_THRESH && leftDistance > 1.0f) {
                park_side = PARK_RIGHT;
                currentParkState = PARK_SCAN_PASSING_WALL;
                Serial.println("-> Wall baseline established on the LEFT side.");
            } 
            else if (rightDistance < SLOT_DETECT_THRESH && rightDistance > 1.0f) {
                park_side = PARK_LEFT;
                currentParkState = PARK_SCAN_PASSING_WALL;
                Serial.println("-> Wall baseline established on the RIGHT side.");
            }
            break;

        case PARK_SCAN_PASSING_WALL:
            moveForward(SCAN_SPEED);
            
            if (active_side_distance > SLOT_DETECT_THRESH || active_side_distance == 0.0f || active_side_distance >= 999.0f) {
                gap_confirm_counter++;
                if (gap_confirm_counter >= 3) { 
                    gap_start_time = currentMillis;
                    gap_confirm_counter = 0;
                    wall_confirm_counter = 0;
                    currentParkState = PARK_SCAN_MEASURING_GAP;
                    Serial.println("-> [OPENING FOUND]: Space detected. Timing clearance window...");
                }
            } else {
                gap_confirm_counter = 0; 
            }
            break;

        case PARK_SCAN_MEASURING_GAP:
            moveForward(SCAN_SPEED);
            if (currentMillis - gap_start_time >= SLOT_MIN_TIME_MS) {
                stopMotors();
                maneuver_timer = currentMillis;
                
                Serial.print("-> [SUCCESS]: Spot found! Distance to curb: ");
                Serial.print(measured_side_distance);
                Serial.println(" cm.");

                if (measured_side_distance < 13.0f) {
                    Serial.println("-> ZONE: CLOSE. Adjusting for tight entry.");
                    dynamic_offset_ms = 400UL;      
                    dynamic_turn1_ms  = 950UL;      
                    dynamic_straight_ms = 950UL;    
                    dynamic_turn2_ms  = 950UL;      
                } 
                else if (measured_side_distance >= 13.0f && measured_side_distance <= 18.0f) {
                    Serial.println("-> ZONE: PERFECT. Using standard baseline timing.");
                    dynamic_offset_ms = 350UL;      
                    dynamic_turn1_ms  = 900UL;      
                    dynamic_straight_ms = 1000UL;   
                    dynamic_turn2_ms  = 900UL;      
                } 
                else {
                    Serial.println("-> ZONE: FAR. Adjusting for deep entry.");
                    dynamic_offset_ms = 300UL;      
                    dynamic_turn1_ms  = 1000UL;     
                    dynamic_straight_ms = 1150UL;   
                    dynamic_turn2_ms  = 1000UL;     
                }
                
                currentParkState = PARK_POSITIONING;
            }
            else if (active_side_distance <= SLOT_DETECT_THRESH && active_side_distance > 1.0f) {
                wall_confirm_counter++;
                if (wall_confirm_counter >= 3) {
                    Serial.println("-> [REJECTED]: Spot too short. Resetting search matrix...");
                    wall_confirm_counter = 0;
                    gap_confirm_counter = 0;
                    currentParkState = PARK_SCAN_PASSING_WALL; 
                }
            } else {
                wall_confirm_counter = 0;
            }
            break;

        case PARK_POSITIONING:         
            if (currentMillis - maneuver_timer >= OFFSET_MS) {
                stopMotors();
                delay(300);
                maneuver_timer = currentMillis;
                currentParkState = PARK_MANEUVER_TURN1;
                Serial.println("-> Step 1: Pivoting rear end into parking space...");
            }
            break;

        case PARK_MANEUVER_TURN1:
            // SWAPPED: Pivot directions
            if (park_side == PARK_LEFT) {
                moveDiagonalBackwardLeft(TURN_SPEED); 
            } else {
                moveDiagonalBackwardRight(TURN_SPEED);  
            }

            if (currentMillis - maneuver_timer >= TURN1_MS) {
                stopMotors();
                delay(300);
                maneuver_timer = currentMillis;
                currentParkState = PARK_MANEUVER_STRAIGHT;
                Serial.println("-> Step 2: Backing up diagonally into space depth...");
            }
            break;

        case PARK_MANEUVER_STRAIGHT:
            moveBackward(TURN_SPEED);
            if (currentMillis - maneuver_timer >= STRAIGHT_MS) {
                stopMotors();
                delay(300);
                maneuver_timer = currentMillis;
                currentParkState = PARK_MANEUVER_TURN2;
                Serial.println("-> Step 3: Counter pivoting front nose straight...");
            }
            break;

        case PARK_MANEUVER_TURN2:
            // SWAPPED: Pivot directions
            if (park_side == PARK_LEFT) {
                moveDiagonalBackwardRight(TURN_SPEED);
            } else {
                moveDiagonalBackwardLeft(TURN_SPEED);
            }

            if (currentMillis - maneuver_timer >= TURN2_MS) {
                stopMotors();
                delay(300);
                maneuver_timer = currentMillis;
                currentParkState = PARK_MANEUVER_CORRECT;
                Serial.println("-> Step 4: Centering car body alignment adjustments...");
            }
            break;

        case PARK_MANEUVER_CORRECT:
        
           if (frontDistance > 15.0f && frontDistance < 20.0f) {
                moveForward(correction);
                delay(200);  // Drive forward for exactly 1 second
                stopMotors();
           }
        
            if (currentMillis - maneuver_timer >= CORRECT_MS) {
               
                Serial.println("=============================================");
                Serial.println("[PARK_OK]: AUTO-PARK MODE EXITED CLEANLY.");
                Serial.println("=============================================");
            }
            break;
 stopAutoPark(); 
        case PARK_DONE:
        case PARK_FAILED:
        case PARK_IDLE:
        default:
            stopMotors(); 
            break;
    }
}