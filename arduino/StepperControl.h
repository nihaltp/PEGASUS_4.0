#ifndef STEPPER_CONTROL_H
#define STEPPER_CONTROL_H

void initSteppers();
void moveCamera(int steps);
void moveX(int steps, bool dir);
void moveY(int steps, bool dir);

#endif
