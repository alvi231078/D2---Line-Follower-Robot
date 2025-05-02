// === Motor pin configuration ===
const int motorL1 = 9;    // Left motor forward
const int motorL2 = 10;   // Left motor backward
const int motorR1 = 5;    // Right motor forward
const int motorR2 = 6;    // Right motor backward

// === Ultrasonic distance sensor pins ===
const int trigPin = 7;    // Sends pulse (trigger)
const int echoPin = 8;    // Receives echo (response)

// === IR sensors for line following ===
const int irLeft  = 2;    // Left IR sensor
const int irRight = 3;    // Right IR sensor

// === Thresholds and limits ===
const int obstacleThreshold = 30;      // Distance in cm to detect an obstacle
const long recoveryTimeout  = 3000;    // Time limit to recover lost line (ms)
const long avoidTimeout     = 5000;    // Max time to avoid an obstacle (ms)

void setup() {
  // Configure motor pins as outputs
  pinMode(motorL1, OUTPUT);
  pinMode(motorL2, OUTPUT);
  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);

  // Set ultrasonic pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Set IR sensor pins
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);

  // Begin serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  long distance = getDistance();          // Measure distance to obstacle
  bool leftIR = digitalRead(irLeft);      // Check left IR sensor
  bool rightIR = digitalRead(irRight);    // Check right IR sensor

  printStatus(distance, leftIR, rightIR); // Log current sensor states

  // Obstacle detected ahead
  if (distance < obstacleThreshold) {
    Serial.println("Obstacle detected. Initiating avoidance maneuver.");
    avoid();
    return;
  }

  // Interpret line sensor data
  if (!leftIR && !rightIR) {
    Serial.println("Line centered. Moving forward.");
    moveForward();
  } else if (!leftIR && rightIR) {
    Serial.println("Line drifting right. Adjusting left.");
    turnLeft();
  } else if (leftIR && !rightIR) {
    Serial.println("Line drifting left. Adjusting right.");
    turnRight();
  } else {
    Serial.println("Line lost. Attempting recovery.");
    recover();
  }

  delay(100); // Short pause to stabilize readings
}

// === Distance measurement using ultrasonic sensor ===
long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 20000); // Timeout for safety
  if (duration == 0) return 999;                // Return large value if no echo

  return duration * 0.034 / 2;  // Convert to centimeters
}

// === Print distance and IR sensor values to serial ===
void printStatus(long dist, bool left, bool right) {
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.println(" cm");

  Serial.print("IR Left: ");
  Serial.print(left ? "WHITE" : "BLACK");

  Serial.print(" | IR Right: ");
  Serial.println(right ? "WHITE" : "BLACK");
}

// === Movement functions ===
void moveForward() {
  digitalWrite(motorL1, HIGH);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, HIGH);
  digitalWrite(motorR2, LOW);
}

void stopMotors() {
  digitalWrite(motorL1, LOW);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, LOW);
}

void turnLeft() {
  digitalWrite(motorL1, LOW);
  digitalWrite(motorL2, HIGH);
  digitalWrite(motorR1, HIGH);
  digitalWrite(motorR2, LOW);
}

void turnRight() {
  digitalWrite(motorL1, HIGH);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, HIGH);
}

// === Recovery behavior if line is lost ===
void recover() {
  stopMotors();
  delay(100);

  // Start spinning in place (clockwise)
  digitalWrite(motorL1, HIGH);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, HIGH);

  unsigned long start = millis();

  // Spin until line is detected again or timeout occurs
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

// === Obstacle avoidance sequence ===
void avoid() {
  unsigned long start = millis();

  // Step 1: Turn right slightly
  turnRight();
  delay(400);

  // Step 2: Move forward for a short duration or until obstacle is cleared
  moveForward();
  unsigned long forwardStart = millis();
  while (millis() - forwardStart < 1200) {
    if (getDistance() > obstacleThreshold + 10) break;
    delay(50);
  }

  // Step 3: Turn back slightly to re-align
  turnLeft();
  delay(400);

  // Step 4: Move forward again to resume line
  moveForward();
  delay(500);

  // Attempt to reacquire line
  recover();

  // If still stuck after timeout, just proceed forward
  if (millis() - start > avoidTimeout) {
    Serial.println("Avoidance timeout reached. Proceeding forward.");
    moveForward();
    delay(1000);
  }
}
