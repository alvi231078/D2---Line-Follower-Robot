// === Motor Pin Configuration ===
const int motorL1 = 9;    // Left motor forward
const int motorL2 = 10;   // Left motor backward
const int motorR1 = 5;    // Right motor forward
const int motorR2 = 6;    // Right motor backward
const int motorSpeed = 150; // PWM Speed (0–255)

// === Ultrasonic Sensor Pins ===
const int trigPin = 7;    
const int echoPin = 8;    

// === IR Sensors ===
const int irLeft  = 2;
const int irRight = 3;

// === Thresholds & Timings ===
const int obstacleThreshold = 30;      // Distance in cm
const long recoveryTimeout  = 3000;    // Line recovery timeout
const long avoidTimeout     = 5000;    // Obstacle avoid timeout

void setup() {
  // Initialize motor pins
  pinMode(motorL1, OUTPUT);
  pinMode(motorL2, OUTPUT);
  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);

  // Initialize sensors
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);

  Serial.begin(9600);
}

void loop() {
  long distance = getDistanceCM();
  bool leftDetected = isLineUnderSensor(irLeft);
  bool rightDetected = isLineUnderSensor(irRight);

  logStatus(distance, leftDetected, rightDetected);

  if (distance < obstacleThreshold) {
    Serial.println("[!] Obstacle ahead. Attempting avoidance...");
    avoidObstacle();
    return;
  }

  // Line following logic
  if (!leftDetected && !rightDetected) {
    Serial.println("[✓] Line centered. Moving forward.");
    moveForward();
  } else if (!leftDetected && rightDetected) {
    Serial.println("[→] Drifting right. Adjusting left.");
    turnLeft();
  } else if (leftDetected && !rightDetected) {
    Serial.println("[←] Drifting left. Adjusting right.");
    turnRight();
  } else {
    Serial.println("[X] Line lost. Entering recovery mode.");
    recoverLine();
  }

  delay(100); // Loop delay for stability
}

// === Sensor Functions ===

long getDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 20000); // timeout to avoid blocking
  if (duration == 0) return 999;  // no echo

  return duration * 0.034 / 2; // convert to cm
}

bool isLineUnderSensor(int pin) {
  // Assumes active-low IR sensors (returns LOW when over black line)
  return digitalRead(pin) == LOW;
}

// === Logging ===

void logStatus(long distance, bool left, bool right) {
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("IR Left: ");
  Serial.print(left ? "BLACK" : "WHITE");
  Serial.print(" | IR Right: ");
  Serial.println(right ? "BLACK" : "WHITE");
}

// === Movement Control ===

void moveForward() {
  analogWrite(motorL1, motorSpeed);
  digitalWrite(motorL2, LOW);
  analogWrite(motorR1, motorSpeed);
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
  analogWrite(motorL2, motorSpeed);
  analogWrite(motorR1, motorSpeed);
  digitalWrite(motorR2, LOW);
}

void turnRight() {
  analogWrite(motorL1, motorSpeed);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);
  analogWrite(motorR2, motorSpeed);
}

// === Behavior Logic ===

void recoverLine() {
  stopMotors();
  delay(100);

  // Spin in place to find the line again
  analogWrite(motorL1, motorSpeed);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);
  analogWrite(motorR2, motorSpeed);

  unsigned long start = millis();
  while (!isLineUnderSensor(irLeft) && !isLineUnderSensor(irRight)) {
    if (millis() - start > recoveryTimeout) {
      Serial.println("[-] Failed to recover line. Stopping.");
      stopMotors();
      return;
    }
    delay(20);
  }

  stopMotors();
  Serial.println("[✓] Line found and recovered.");
}

void avoidObstacle() {
  unsigned long start = millis();

  // Step 1: Turn slightly right
  turnRight();
  delay(400);

  // Step 2: Go forward for a moment
  moveForward();
  unsigned long forwardStart = millis();
  while (millis() - forwardStart < 1200) {
    if (getDistanceCM() > obstacleThreshold + 10) break;
    delay(50);
  }

  // Step 3: Turn slightly left to realign
  turnLeft();
  delay(400);

  // Step 4: Move forward a bit more
  moveForward();
  delay(500);

  // Try recovering the line after avoidance
  recoverLine();

  // Timeout fallback
  if (millis() - start > avoidTimeout) {
    Serial.println("[!] Avoidance timeout. Proceeding forward anyway.");
    moveForward();
    delay(1000);
  }
}
