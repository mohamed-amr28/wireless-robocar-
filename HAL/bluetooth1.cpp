#include "bluetooth1.h"
#include "motor1.h"
#include "../APP/manual1.h"      
#include "ultrasonic1.h"
#include "../APP/autopark1.h"    
#include "../APP/savepath1.h"  

#include <Arduino.h>

char currentMode = 'M';       
float currentSpeed = 0.0;   

extern void manualmode(char cmd); 

void Bluetooth_Init(void){
  Serial.begin(9600); 
}

char Bluetooth_ReadCommand(void){
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c != '\n' && c != '\r' && c != ' ') {
      return c;
    }
  }
  return '\0'; 
}

void Bluetooth_Send(const char* message){
  Serial.println(message);
}

void handleBluetooth() {
  char cmd = Bluetooth_ReadCommand();
  
  if (cmd != '\0') {
    // Speed mapping
    if (cmd >= '0' && cmd <= '9') {
      int level = cmd - '0';
      if (level == 0) {
        currentSpeed = 0; 
      } else {
        const int MIN_PWM = 130; 
        currentSpeed = map(level, 1, 9, MIN_PWM, 255);
      }
    } 
    // Mode switching
    else if (cmd == 'X') {
      currentMode = 'A'; 
    } 
    else if (cmd == 'P') {
      currentMode = 'P';
      stopMotors();
      initAutoPark();
    } 
    else if (cmd == 'm') {
      currentMode = 'M'; 
      stopMotors(); 
    }
    // Save Path commands
    else if (cmd == 'K') {
      currentMode = 'M';
      startRecording();
    }
    else if (cmd == 'T') {
      stopRecording();
    }
    else if (cmd == 'E') {
      startPlayback();
    }
    else if (cmd == 'Q') {
      stopPlayback();
      currentMode = 'M';
    }
    // Manual movement
    else {
      if (currentMode == 'M') {
        savepath_onCommand(cmd);
        manualmode(cmd); 
      }
    }
  }
}

void sendSensorData(){
  static unsigned long lastSend = 0;
  if (millis() - lastSend >= 500) { 
    lastSend = millis();
    Serial.print("SNS:");
    Serial.print((int)leftDistance);
    Serial.print(",");
    Serial.print((int)frontDistance);
    Serial.print(",");
    Serial.println((int)rightDistance);
  }
}
