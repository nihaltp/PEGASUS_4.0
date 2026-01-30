#include <Arduino.h>
#include <Servo.h>
#include "ServoControl.h"
#include "Pins.h"
#include "PositionTracker.h"

Servo toolServo;

void initServo() {
  toolServo.attach(SERVO_PIN);
  toolServo.write(90); // neutral position
  initServoAngle(90);
}

void setServoAngle(int angle) {
  angle = constrain(angle, 0, 180);
  toolServo.write(angle);
  initServoAngle(angle);
}
