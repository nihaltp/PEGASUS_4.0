#include "Arduino.h"
#include "MotionPlanner.h"
#include "StepperControl.h"

void moveTo(int xSteps, int ySteps) {
  if (xSteps != 0) {
    moveCameraStepper(abs(xSteps), xSteps > 0);
  }

  if (ySteps != 0) {
    moveLaserStepper(abs(ySteps), ySteps > 0);
  }
}
