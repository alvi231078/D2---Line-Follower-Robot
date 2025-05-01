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

// Safety constants
#define OBSTACLE_DISTANCE_THRESHOLD 30   // cm
#define RECOVERY_TIMEOUT 3000            // ms
#define AVOID_TIMEOUT 5000               // ms

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

  if (distance < OBSTACLE_DISTANCE_THRESHOLD) {
    Serial.println("⚠️ Obstacle detected! Attempting bypass...\n");
    avoidObstacle();
    return;
  }

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
    Serial.println("🚫 Lost line – recovering...\n");
    recoverLine();
  }

  delay(100);
}

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 20000); // timeout 20ms
  if (duration == 0) return 999;  // treat as very far
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

// Smart bypass maneuver with timeout & checks
void avoidObstacle() {
  unsigned long start = millis();

  // Step 1: Turn right
  turnRight();
  delay(400);

  // Step 2: Move forward beside obstacle
  moveForward();
  unsigned long forwardStart = millis();
  while (millis() - forwardStart < 1200) {
    if (getDistance() > OBSTACLE_DISTANCE_THRESHOLD + 10) break;
    delay(50);
  }

  // Step 3: Turn back left
  turnLeft();
  delay(400);

  // Step 4: Move forward to re-align
  moveForward();
  delay(500);

  // Step 5: Try to re-enter line
  recoverLine();

  // Fail-safe timeout
  if (millis() - start > AVOID_TIMEOUT) {
    Serial.println("❌ Obstacle bypass timeout. Resuming forward.\n");
    moveForward();
    delay(1000);
  }
}

// Safe recoverLine() with timeout
void recoverLine() {
  stopMotors();
  delay(100);

  // Spin right to search
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);

  unsigned long startTime = millis();
  while (digitalRead(IR_LEFT) == HIGH && digitalRead(IR_RIGHT) == HIGH) {
    if (millis() - startTime > RECOVERY_TIMEOUT) {
      Serial.println("❌ Recovery failed: timeout. Stopping...\n");
      stopMotors();
      return;
    }
    delay(20);
  }

  stopMotors();
  Serial.println("✅ Line re-acquired.\n");
}
