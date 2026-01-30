#include "Arduino.h"
#include "MotionPlanner.h"
#include "ServoControl.h"

void moveInterruptible(int targetX, int targetAngle);

/**
 * Traces a rectangle with the laser.
 * Stepper (X-axis) rotates the base.
 * Servo (Y-axis) moves the laser up and down.
 * @param cx Center X (not used in this specific trace logic)
 * @param cy Center Y (not used in this specific trace logic)
 * @param x1 Top-left X coordinate (stepper steps)
 * @param y1 Top-left Y coordinate (servo angle)
 * @param x2 Bottom-right X coordinate (stepper steps)
 * @param y2 Bottom-right Y coordinate (servo angle)
 */
void targetBird(int cx, int cy, int x1, int y1, int x2, int y2) {
    int minX = x1;
    int maxX = x2;
    int minY = y1;
    int maxY = y2;

    float maxRx = (maxX - minX) * 0.5;
    float maxRy = (maxY - minY) * 0.5;

    static float theta = 0.0;
    static float r = 0.0;

    static int lastMinX = 0, lastMinY = 0, lastMaxX = 0, lastMaxY = 0;

    // Reset spiral if bounding box changes significantly
    if (minX != lastMinX || minY != lastMinY ||
        maxX != lastMaxX || maxY != lastMaxY) {
        theta = 0.0;
        r = 0.0;
    }

    lastMinX = minX;
    lastMinY = minY;
    lastMaxX = maxX;
    lastMaxY = maxY;

    theta += 0.05;      // angular speed
    r += 0.0005;        // expansion speed

    if (r > 1.0) r = 0.0;

    float fx = cx + cos(theta) * maxRx * r;
    float fy = cy + sin(theta) * maxRy * r;

    int x = constrain((int)fx, minX, maxX);
    int y = constrain((int)fy, minY, maxY);

    moveInterruptible(x, y);
}

void randomMovement() {
    int minStepper = -500;
    int maxStepper = 500;

    int minServo = 45;
    int maxServo = 135;

    bool direction = true;

    for (int x = minServo; x < maxServo; x += 10) {
        if (direction) {
            for (int y = minStepper; y < maxStepper; y += 50) {
                moveInterruptible(y, x);
            }
        } else {
            for (int y = maxStepper; y > minStepper; y -= 50) {
                moveInterruptible(y, x);
            }
        }
        direction = !direction;
    }
}

void moveInterruptible(int targetX, int targetAngle) {
    // Basic logic: move toward target, but abort if serial data arrives
    if (Serial.available() > 0) return; 

    moveTo(targetX, 0); // Horizontal
    setServoAngle(targetAngle); // Vertical
    
    // Replace a single long delay(1000) with a serial-aware wait
    for(int i = 0; i < 10; i++) {
        if (Serial.available() > 0) break;
        delay(50);
    }
}
