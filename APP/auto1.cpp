#include "../HAL/bluetooth1.h"
#include "../HAL/motor1.h"      
#include "../HAL/battery1.h"
#include "../HAL/ultrasonic1.h" 
#include "auto1.h"
//AUTO
    // wait some time until find other path

// MUST use extern here so it shares the speed with bluetooth!
   

void runAutonomousMode() {

  updateallsensors();
  
  // Fixed variable names from your ultrasonic.h file
  if (frontDistance > SAFE_DISTANCE || frontDistance == 0) {
    moveForward(auto_speed);
  } 
  else {
    // Path is blocked! 
    stopMotors();
    delay(SCAN_DELAY); 
    
    if (leftDistance > rightDistance && leftDistance > SAFE_DISTANCE) {
      turnLeft(autoturn_speed);
      delay(TURN_TIME); 
      stopMotors();
    } 
    else if (rightDistance >= leftDistance && rightDistance > SAFE_DISTANCE) {
      turnRight(autoturn_speed);
      delay(TURN_TIME);
      stopMotors();
    } 
    else if (rightDistance<SAFE_DISTANCE && leftDistance<SAFE_DISTANCE) {
      // No clear path, reverse
      while(rightDistance<SAFE_DISTANCE && leftDistance<SAFE_DISTANCE)
      {
      moveBackward(auto_speed);
      delay(TURN_TIME);
      stopMotors();
      updateallsensors();
      }
    if(leftDistance>rightDistance){
      turnLeft(autoturn_speed);
      delay(TURN_TIME); 
      stopMotors();
    }
    else{
      turnRight(autoturn_speed);
      delay(TURN_TIME); 
      stopMotors();

    }
    }
  }
}