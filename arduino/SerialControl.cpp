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
