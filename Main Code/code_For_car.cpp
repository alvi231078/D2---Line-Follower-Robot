// === Motor pin configuration ===
const int motorL1 = 9;    // Left motor forward
const int motorL2 = 10;   // Left motor backward
const int motorR1 = 5;    // Right motor forward
const int motorR2 = 6;    // Right motor backward

// === Ultrasonic distance sensor pins ===
const int trigPin = 7;    // Trigger pin (sends pulse)
const int echoPin = 8;    // Echo pin (receives pulse)

// === IR sensors for line following ===
const int irLeft  = 2;    // Left IR sensor
const int irRight = 3;    // Right IR sensor

// === Thresholds and limits ===
const int obstacleThreshold = 30;      // Distance below which an obstacle is considered "too close" (in cm)
const long recoveryTimeout  = 3000;    // Max time (ms) to search for a lost line
const long avoidTimeout     = 5000;    // Max time (ms) to try avoiding an obstacle

void setup() {
  pinMode(motorL1, OUTPUT);
  pinMode(motorL2, OUTPUT);
  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);

  Serial.begin(9600);
}

void loop() {
  long distance = getDistance();
  bool leftIR = digitalRead(irLeft);
  bool rightIR = digitalRead(irRight);

  printStatus(distance, leftIR, rightIR);

  if (distance < obstacleThreshold) {
    Serial.println("Obstacle detected. Initiating avoidance maneuver.");
    avoid();
    return;
  }

  if (!leftIR && !rightIR) {
    Serial.println("Line detected under both sensors. Moving forward.");
    moveForward();
  } else if (!leftIR && rightIR) {
    Serial.println("Right drift detected. Adjusting left.");
    turnLeft();
  } else if (leftIR && !rightIR) {
    Serial.println("Left drift detected. Adjusting right.");
    turnRight();
  } else {
    Serial.println("No line detected. Attempting to recover.");
    recover();
  }

  delay(100);
}

// Sends a pulse and listens for echo to measure distance in centimeters
long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 20000);
  if (duration == 0) return 999;

  return duration * 0.034 / 2;
}

// Debug info for distance and IR sensors
void printStatus(long dist, bool left, bool right) {
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.println(" cm");

  Serial.print("IR Left: ");
  Serial.print(left ? "WHITE" : "BLACK");

  Serial.print(" | IR Right: ");
  Serial.println(right ? "WHITE" : "BLACK");
}

// Moves the robot forward by setting both motors to forward
void moveForward() {
  digitalWrite(motorL1, HIGH);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, HIGH);
  digitalWrite(motorR2, LOW);
}

// Stops all motors
void stopMotors() {
  digitalWrite(motorL1, LOW);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, LOW);
}

// Spins the robot to the left
void turnLeft() {
  digitalWrite(motorL1, LOW);
  digitalWrite(motorL2, HIGH);
  digitalWrite(motorR1, HIGH);
  digitalWrite(motorR2, LOW);
}

// Spins the robot to the right
void turnRight() {
  digitalWrite(motorL1, HIGH);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, HIGH);
}

// If both IR sensors see white, the robot lost the line and spins until it finds it again
void recover() {
  stopMotors();
  delay(100);

  // Begin spinning clockwise
  digitalWrite(motorL1, HIGH);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, HIGH);

  unsigned long start = millis();

  // Spin until at least one sensor sees black or timeout is hit
  while (digitalRead(irLeft) == HIGH && digitalRead(irRight) == HIGH) {
    if (millis() - start > recoveryTimeout) {
      Serial.println("Line recovery failed. Stopping.");
      stopMotors();
      return;
    }
    delay(20);
  }

  stopMotors();
  Serial.println("Line successfully recovered.");
}

// Obstacle avoidance routine using a predefined sequence of turns and forward motion
void avoid() {
  unsigned long start = millis();

  turnRight();
  delay(400);

  moveForward();
  unsigned long forwardStart = millis();
  while (millis() - forwardStart < 1200) {
    if (getDistance() > obstacleThreshold + 10) break;
    delay(50);
  }

  turnLeft();
  delay(400);

  moveForward();
  delay(500);

  recover();

  if (millis() - start > avoidTimeout) {
    Serial.println("Avoidance timeout reached. Proceeding forward anyway.");
    moveForward();
    delay(1000);
  }
}
