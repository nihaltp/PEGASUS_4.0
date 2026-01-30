#include "Arduino.h"
#include "MotionPlanner.h"
#include "ServoControl.h"

void moveInterruptible(int targetX, int targetAngle);

/**
 * Traces a rectangle with the laser.
 * Stepper (X-axis) rotates the base.
 * Servo (Y-axis) moves the laser up and down.
 * @param cx Current X (not used in this specific trace logic)
 * @param cy Current Y (not used in this specific trace logic)
 * @param x1 Top-left X coordinate (stepper steps)
 * @param y1 Top-left Y coordinate (servo angle)
 * @param x2 Bottom-right X coordinate (stepper steps)
 * @param y2 Bottom-right Y coordinate (servo angle)
 */
void targetBird(int cx, int cy, int x1, int y1, int x2, int y2) {
    // 1. Move to the starting corner (x1, y1)
    // moveTo handles the stepper position
    // setServoAngle handles the vertical tilt
    moveTo(x1, 0);
    setServoAngle(y1);
    delay(100); // Brief pause for stability

    // 2. Trace to (x2, y1) - Move stepper across
    moveTo(x2, 0);
    delay(100);

    // 3. Trace to (x2, y2) - Move servo down/up
    setServoAngle(y2);
    delay(100);

    // 4. Trace to (x1, y2) - Move stepper back
    moveTo(x1, 0);
    delay(100);

    // 5. Trace back to (x1, y1) - Move servo back to start
    setServoAngle(y1);
    delay(100);
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
