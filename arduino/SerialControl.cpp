#include <Arduino.h>
#include "SerialControl.h"
#include "StepperControl.h"
#include "ServoControl.h"
#include "MotionPlanner.h"
#include "Target.h"

#define SERIAL_BAUD 115200
#define BUFFER_SIZE 64

static char buffer[BUFFER_SIZE];
static uint8_t index = 0;

void handleCommand(char* cmd);

void initSerial() {
  Serial.begin(SERIAL_BAUD);
  while (!Serial) {
    ; // wait for serial on some boards
  }
}

void processSerial() {
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n') {
      buffer[index] = '\0';
      handleCommand(buffer);
      index = 0;
    }
    else if (index < BUFFER_SIZE - 1) {
      buffer[index++] = c;
    }
  }
}

void handleCommand(char* cmd) {
  // MOVE {steps}
  if (strncmp(cmd, "MOVE", 4) == 0) {

    char* valuePtr = strchr(cmd, ' ');
    if (!valuePtr) {
      Serial.println("ERR: MOVE needs value");
      return;
    }

    int steps = atoi(valuePtr + 1);

    Serial.print("MOVE command received");

    moveCamera(steps);
    return;
  }

  // Bird detected | center=({cx},{cy}) | x1,y1=({x1},{y1}) | x2,y2=({x2},{y2})
  if (strncmp(cmd, "Bird detected", 13) == 0) {

    int cx, cy;
    int x1, y1;
    int x2, y2;

    int matched = sscanf(
      cmd,
      "Bird detected | center=(%d,%d) | x1,y1=(%d,%d) | x2,y2=(%d,%d)",
      &cx, &cy,
      &x1, &y1,
      &x2, &y2
    );

    if (matched != 6) {
      Serial.println("ERR: Failed to parse Bird detected data");
      return;
    }

    targetBird(cx, cy, x1, y1, x2, y2);
    return;
  }

  // ---- Unknown command ----
  Serial.print("ERR: Unknown command -> ");
  Serial.println(cmd);
}
