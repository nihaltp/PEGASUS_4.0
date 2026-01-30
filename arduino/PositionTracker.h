#ifndef POSITION_TRACKER_H
#define POSITION_TRACKER_H

// Initializers
void initPositionTracker();

void initCameraPOS(long steps);
void initLaserPOS(long steps);
void initServoAngle(int angle);

// Getters for current state
long getCameraPOS();
long getLaserPOS();
int getServoAngle();

// Setters/Updaters
void updateCameraPOS(long steps);
void updateLaserPOS(long steps);
void updateServoAngle(int angle);

void addCameraStep();
void addLaserStep();
void addServoAngle();

#endif
