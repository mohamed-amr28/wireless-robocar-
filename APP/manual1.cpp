#include "../HAL/bluetooth1.h"
#include "../HAL/motor1.h"
#include "../HAL/battery1.h"
#include "manual1.h"

void manualmode(char comment){
    switch (comment) {
        // Move forward
        case 'F': 
        case 'f':
            moveForward(currentSpeed);  
            break;
            
        // Move backward
        case 'B': 
        case 'b':
            moveBackward(currentSpeed); 
            break;
            
        // Turn left
        case 'L': 
        case 'l':
            turnLeft(currentSpeed);     
            break;
            
        // Turn right
        case 'R': 
        case 'r':
            turnRight(currentSpeed);    
            break;

        // --- DIAGONAL MOVEMENT CASES ---
        case 'I': // Forward Right
        case 'g':
            moveDiagonalForwardRight(currentSpeed);
            break;

        case 'G': // Forward Left
        case 'i':
            moveDiagonalForwardLeft(currentSpeed);
            break;

        case 'J': // Backward Right
        case 'h':
            moveDiagonalBackwardRight(currentSpeed);
            break;

        case 'H': // Backward Left
        case 'j':
            moveDiagonalBackwardLeft(currentSpeed);
            break;
            
        // Stop all motors
        case 'S': 
        case 's':
            stopMotors();               
            break; 

        default:
            // Safely ignore any unmapped app inputs
            break;
    }
}