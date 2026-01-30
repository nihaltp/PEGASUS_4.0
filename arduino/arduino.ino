#include "StepperControl.h"
#include "ServoControl.h"
#include "MotionPlanner.h"

void setup() {
  initSteppers();
  initServo();
}

void loop() {
  processSerial();
}
