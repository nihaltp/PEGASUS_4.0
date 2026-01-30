#ifndef STEPPER_CONTROL_H
#define STEPPER_CONTROL_H

void initSteppers();
void moveCamera(int steps);
void moveCameraStepper(int steps, bool dir);
void moveLaserStepper(int steps, bool dir);

#endif
