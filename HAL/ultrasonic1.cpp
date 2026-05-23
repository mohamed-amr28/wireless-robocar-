#include "ultrasonic1.h"

// Define them here! This creates exactly ONE copy of the variables in memory.
long duration;
float distance;
float frontDistance = 0;
float leftDistance  = 0;
float rightDistance = 0;

float readDistance(int trigPin, int echoPin, float previousValue)
{
  // Clear trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send 10us pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo pulse
  duration = pulseIn(echoPin, HIGH, 30000);

  // If timeout happened (OPEN SPACE DETECTED)
  if(duration == 0)
  {
    return 999.0f; // <-- CHANGE THIS from previousValue to 999.0f
  }

  // Convert to cm
  distance = duration * 0.0343 / 2;

  // Filter invalid values
  if(distance < 2 || distance > 1000)
  {
    return 999;
  }

  return distance;
}

void updateallsensors() {
  frontDistance = readDistance(TRIG_FRONT, ECHO_FRONT, frontDistance);
  delay(10); 
  leftDistance  = readDistance(TRIG_LEFT, ECHO_LEFT, leftDistance);
  delay(10);
  rightDistance = readDistance(TRIG_RIGHT, ECHO_RIGHT, rightDistance);
}

void setupSensors() {
  pinMode(TRIG_FRONT, OUTPUT); pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_LEFT, OUTPUT);  pinMode(ECHO_LEFT, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT); pinMode(ECHO_RIGHT, INPUT);
}