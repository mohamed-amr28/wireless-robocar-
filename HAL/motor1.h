#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>

const int IN1 = 2;
const int ENA = 3; 
const int IN2 = 4;

// Right Side Motors
const int IN3 = 5;
const int IN4 = 6;
const int ENB = 9; 

void setupMotors();
void moveForward(int speed);
void moveBackward(int speed);
void turnLeft(int speed);
void turnRight(int speed);
void stopMotors();

// --- DIAGONAL FUNCTIONS ---
void moveDiagonalForwardRight(int speed);
void moveDiagonalForwardLeft(int speed);
void moveDiagonalBackwardRight(int speed);
void moveDiagonalBackwardLeft(int speed);


#endif // MOTOR_H