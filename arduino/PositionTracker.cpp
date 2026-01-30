#include "PositionTracker.h"
#include "Arduino.h"

// Global variables (will reset on power-loss)
static long currentCameraPOS = 0;
static long currentLaserPOS = 0;
static int currentServoAngle = 90; // Default neutral

void initPositionTracker() { initCameraPOS(0); initLaserPOS(0); initServoAngle(90); }

void initCameraPOS(long steps = 0) { currentCameraPOS = steps; }
void initLaserPOS(long steps = 0) { currentLaserPOS = steps; }
void initServoAngle(int angle = 90) { currentServoAngle = constrain(angle, 0, 180); }

long getCameraPOS() { return currentCameraPOS; }
long getLaserPOS() { return currentLaserPOS; }
int getServoAngle() { return currentServoAngle; }

void updateCameraPOS(long steps) { currentCameraPOS += steps; }
void updateLaserPOS(long steps) { currentLaserPOS += steps; }
void updateServoAngle(int angle) {
  int temp_angle = constrain(currentServoAngle + angle, 0, 180);
  currentServoAngle = temp_angle;
}

void addCameraStep() { currentCameraPOS += 1; }
void addLaserStep() { currentLaserPOS += 1; }
void addServoAngle() {
  int temp_angle = constrain(currentServoAngle + 1, 0, 180);
  currentServoAngle = temp_angle;
}
