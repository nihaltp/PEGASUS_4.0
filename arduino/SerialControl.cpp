#include <Arduino.h>
#include "SerialControl.h"
#include "StepperControl.h"
#include "ServoControl.h"
#include "MotionPlanner.h"

#define SERIAL_BAUD 115200
#define BUFFER_SIZE 64

static char buffer[BUFFER_SIZE];
static uint8_t index = 0;

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
  // Examples:
  // MOVE {steps}
  // Bird detected | center=({cx},{cy}) | x1,y1=({x1},{y1}) | x2,y2=({x2},{y2})

  if (cmd.startsWith("MOVE")) {
    int spaceIndex = cmd.indexOf(' ');
    if (spaceIndex == -1) {
      Serial.println("ERR: MOVE needs value");
      return;
    }

    int steps = cmd.substring(spaceIndex + 1).toInt();

    Serial.print("MOVE command received");

    moveCamera(steps);
  }
  else if (strncmp(cmd, "Bird detected", 13) == 0) {

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
    else {
      Serial.println("Bird data parsed successfully");
    }
  }
  else {
    Serial.print("ERR: Unknown command -> ");
    Serial.println(cmd);
  }
}
