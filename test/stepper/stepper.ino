#include <Servo.h>

#define STEP_DELAY_US 800  // controls speed

#define CAMERA_STEP_PIN 2
#define CAMERA_DIR_PIN 3
#define SERVO_PIN 9
#define LASER_STEP_PIN 5
#define LASER_DIR_PIN 6
#define LASER_PIN 8

Servo servoMotor;
int servoAngle = 90;
int servoMinAngle = 135;
int servoMaxAngle = 180;

bool laserState = false;
unsigned long lastBlink = 0;
const unsigned long interval = 500; // ms

void blinkLaser();

void setup() {
  pinMode(CAMERA_STEP_PIN, OUTPUT);
  pinMode(CAMERA_DIR_PIN, OUTPUT);
  pinMode(LASER_STEP_PIN, OUTPUT);
  pinMode(LASER_DIR_PIN, OUTPUT);
  pinMode(LASER_PIN, OUTPUT);
  servoMotor.attach(SERVO_PIN);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    processCommand(command);
  }
}

void processCommand(String commandLine) {
  commandLine.trim();

  int start = 0;
  while (true) {
    int sep = commandLine.indexOf(';', start);

    if (sep == -1) {
      processSingleCommand(commandLine.substring(start));
      break;
    }

    processSingleCommand(commandLine.substring(start, sep));
    start = sep + 1;
  }
}

void processSingleCommand(String command) {
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

  } else if (command == "laser") {
    laserState = !laserState;
    digitalWrite(LASER_PIN, laserState ? HIGH : LOW);
    Serial.print("OK: laser ");
    Serial.println(laserState ? "ON" : "OFF");

  } else if (command == "laseron") {
    digitalWrite(LASER_PIN, HIGH);
    Serial.print("OK: laser ON");

  } else if (command == "laseroff") {
    digitalWrite(LASER_PIN, LOW);
    Serial.print("OK: laser OFF");

  } else if (command.startsWith("servo ")) {
    int angle = command.substring(6).toInt();
    moveServo(angle);
    Serial.print("OK: servo ");
    Serial.println(angle);

  } else if (command.startsWith("servolaser ")) {
    int angle = command.substring(10).toInt();
    Serial.print("OK: servolaser ");
    Serial.println(angle);
    moveServoLaser(angle);

} else if (command.startsWith("laserxx ")) {
  int firstSpace = command.indexOf(' ');
  int secondSpace = command.indexOf(' ', firstSpace + 1);

  if (secondSpace == -1) {
    Serial.print("ERR: invalid laserxx command: ");
    Serial.println(command);
    return;
  }

  int x1 = command.substring(firstSpace + 1, secondSpace).toInt();
  int x2 = command.substring(secondSpace + 1).toInt();

    Serial.print("OK: laserxx ");
    Serial.print(x1);
    Serial.print(" ");
    Serial.println(x2);
    moveLaserxx(x1, x2);

  } else if (command.startsWith("wait ")) {
    int duration = command.substring(5).toInt();
    delay(duration);
    Serial.print("OK: wait ");
    Serial.println(duration);

  } else {
    Serial.print("ERR: unknown command: ");
    Serial.println(command);
  }
}

void moveServo(int angle) {
  servoMotor.write(angle);
  servoAngle = angle;
}

void moveServoLaser(int angle) {
  for (int i = servoAngle; i != angle; ) {
    if (i < angle) {
      i++;
    } else {
      i--;
    }
    servoMotor.write(i);
    blinkLaser();
    delay(15);
  }
  servoAngle = angle;
  laserState = false;
  digitalWrite(LASER_PIN, LOW);
}

void moveLaserxx(int x1, int x2) {
  moveLaser(x1);
  int laserStep = 10;
  if (x2 < x1) {
    laserStep = -laserStep;
  }
  int servoStep = (servoMaxAngle > servoMinAngle) ? 1 : -1;
  moveServoLaser(servoMinAngle);
  for (int angle = servoMinAngle; angle != servoMaxAngle; angle += servoStep) {
    moveServoLaser(angle);
    if (laserStep > 0) {
      for (int x = x1; x < x2; x += laserStep) {
        moveLaser(laserStep);
        blinkLaser();
      }
    } else {
      for (int x = x1; x > x2; x += laserStep) {
        moveLaser(laserStep);
        blinkLaser();
      }
    }
  }
  laserState = false;
  digitalWrite(LASER_PIN, LOW);
}

void blinkLaser() {
  if (millis() - lastBlink >= interval) {
    lastBlink = millis();
    digitalWrite(LASER_PIN, laserState ? HIGH : LOW);
    laserState = !laserState;
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
