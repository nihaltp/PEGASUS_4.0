#include "StepperControl.h"
#include "ServoControl.h"
#include "MotionPlanner.h"

void setup() {
  initSteppers();
  initServo();
}

void loop() {
  moveTo(200, 0);      // X forward
  delay(500);

  moveTo(0, 200);      // Y forward
  delay(500);

  setServoAngle(45);   // example action
  delay(1000);

  setServoAngle(90);
  delay(2000);
}
