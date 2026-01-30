#include <Arduino.h>
#include "StepperControl.h"
#include "Pins.h"
#include "PositionTracker.h"

#define STEP_DELAY_US 800   // controls speed

void initSteppers() {
  pinMode(CAMERA_STEP_PIN, OUTPUT);
  pinMode(CAMERA_DIR_PIN, OUTPUT);

  pinMode(LASER_STEP_PIN, OUTPUT);
  pinMode(LASER_DIR_PIN, OUTPUT);
}

void moveCamera(int steps) {
  if (steps > 0) {
    moveCameraStepper(steps, HIGH);  // Move X forward
  } else if (steps < 0) {
    moveCameraStepper(-steps, LOW);  // Move X backward
  }
}

void moveStepper(int stepPin, int dirPin, int steps, bool dir) {
  digitalWrite(dirPin, dir);

  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(STEP_DELAY_US);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(STEP_DELAY_US);
  }
}

void moveCameraStepper(int steps, bool dir) {
  moveStepper(CAMERA_STEP_PIN, CAMERA_DIR_PIN, steps, dir);
  updateCameraPOS(dir ? steps : -steps);
}

void moveLaserStepper(int steps, bool dir) {
  moveStepper(LASER_STEP_PIN, LASER_DIR_PIN, steps, dir);
  updateLaserPOS(dir ? steps : -steps);
}
