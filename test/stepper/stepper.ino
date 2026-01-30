#define STEP_DELAY_US 800   // controls speed

#define CAMERA_STEP_PIN 2
#define CAMERA_DIR_PIN  3

#define LASER_STEP_PIN 5
#define LASER_DIR_PIN  6

void setup() {
  pinMode(CAMERA_STEP_PIN, OUTPUT);
  pinMode(CAMERA_DIR_PIN, OUTPUT);
  pinMode(LASER_STEP_PIN, OUTPUT);
  pinMode(LASER_DIR_PIN, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    processCommand(command);
  }
}

void processCommand(String command) {
  command.trim();

  if (command.startsWith("camera ")) {
    int steps = command.substring(7).toInt();
    moveCamera(steps);
    Serial.print("OK: camera ");
    Serial.println(steps);

  } else if (command.startsWith("laser ")) {
    int steps = command.substring(6).toInt();
    moveLaser(steps);
    Serial.print("OK: laser ");
    Serial.println(steps);

  } else {
    Serial.print("ERR: unknown command: ");
    Serial.println(command);
  }
}

void moveCamera(int steps) {
  if (steps > 0) {
    moveCameraStepper(steps, HIGH);  // Move X forward
  } else if (steps < 0) {
    moveCameraStepper(-steps, LOW);  // Move X backward
  }
}

void moveLaser(int steps) {
  if (steps > 0) {
    moveLaserStepper(steps, HIGH);  // Move Y forward
  } else if (steps < 0) {
    moveLaserStepper(-steps, LOW);  // Move Y backward
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
}

void moveLaserStepper(int steps, bool dir) {
  moveStepper(LASER_STEP_PIN, LASER_DIR_PIN, steps, dir);
}
