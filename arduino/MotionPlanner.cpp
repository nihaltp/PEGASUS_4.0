#include "MotionPlanner.h"
#include "StepperControl.h"

void moveTo(int xSteps, int ySteps) {
  if (xSteps != 0) {
    moveX(abs(xSteps), xSteps > 0);
  }

  if (ySteps != 0) {
    moveY(abs(ySteps), ySteps > 0);
  }
}
