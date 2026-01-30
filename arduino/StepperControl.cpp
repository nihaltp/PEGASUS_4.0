#include <Arduino.h>
#include "StepperControl.h"
#include "Pins.h"

#define STEP_DELAY_US 800   // controls speed

void initSteppers() {
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);

  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
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

void moveX(int steps, bool dir) {
  moveStepper(X_STEP_PIN, X_DIR_PIN, steps, dir);
}

void moveY(int steps, bool dir) {
  moveStepper(Y_STEP_PIN, Y_DIR_PIN, steps, dir);
}
