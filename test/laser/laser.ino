#define LASER_PIN 8

void setup() {
  pinMode(LASER_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LASER_PIN, HIGH); // Laser ON
  delay(1000);                   // 1 second

  digitalWrite(LASER_PIN, LOW);  // Laser OFF
  delay(1000);                   // 1 second
}
