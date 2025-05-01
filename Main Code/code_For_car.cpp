// Motor pins
#define motorPin1 9
#define motorPin2 10
#define motorPin3 5
#define motorPin4 6

// Ultrasonic sensor pins
#define TRIG_PIN 7
#define ECHO_PIN 8

// IR sensor pins
#define IR_LEFT 2
#define IR_RIGHT 3

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  Serial.begin(9600);
}

void loop() {
  long distance = getDistance();

  bool leftIR = digitalRead(IR_LEFT);   // HIGH = white, LOW = black
  bool rightIR = digitalRead(IR_RIGHT);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("IR Left: ");
  Serial.print(leftIR ? "WHITE" : "BLACK");
  Serial.print(" | IR Right: ");
  Serial.println(rightIR ? "WHITE" : "BLACK");

  // Obstacle detected? Do full bypass
  if (distance < 30) {
    Serial.println("⚠️ Obstacle detected! Executing bypass maneuver...\n");
    avoidObstacle();
    return;
  }

  // Line following logic
  if (!leftIR && !rightIR) {
    Serial.println("✅ On track – moving forward.\n");
    moveForward();
  }
  else if (!leftIR && rightIR) {
    Serial.println("↪️ Correcting – turning left.\n");
    turnLeft();
  }
  else if (leftIR && !rightIR) {
    Serial.println("↩️ Correcting – turning right.\n");
    turnRight();
  }
  else {
    Serial.println("🚫 Lost line – starting recovery...\n");
    recoverLine();
  }

  delay(150);
}

// Ultrasonic distance in cm
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

// Motion
void moveForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void stopMotors() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}

void turnLeft() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void turnRight() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
}

// Bypass obstacle and re-enter the line
void avoidObstacle() {
  // Step 1: Turn right to leave the line
  turnRight();
  delay(400);

  // Step 2: Drive forward to pass obstacle
  moveForward();
  delay(800);

  // Step 3: Turn left to align with the line again
  turnLeft();
  delay(400);

  // Step 4: Move forward and try to reacquire line
  moveForward();
  delay(600);

  // Step 5: Try to find the line again
  recoverLine();
}

// Line recovery
void recoverLine() {
  stopMotors();
  delay(100);

  // Spin slowly until one sensor sees line
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);

  while (digitalRead(IR_LEFT) == HIGH && digitalRead(IR_RIGHT) == HIGH) {
    Serial.println("🔄 Searching for line...");
    delay(100);
  }

  stopMotors();
  Serial.println("✅ Line re-acquired.\n");
}
