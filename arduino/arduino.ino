#include "StepperControl.h"
#include "ServoControl.h"
#include "MotionPlanner.h"
#include "SerialControl.h"

void setup() {
  initSteppers();
  initServo();
}

void loop() {
  processSerial();
}
