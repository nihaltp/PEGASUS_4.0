#include "StepperControl.h"
#include "ServoControl.h"
#include "MotionPlanner.h"
#include "SerialControl.h"
#include "Target.h"
#include "PositionTracker.h"

void setup() {
  initSteppers();
  initServo();
  initSerial();
  initPositionTracker();
}

void loop() {
  if (Serial.available() > 0) {
    processSerial();
  } else {
    randomMovement();
  }
}
