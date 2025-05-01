// === Motor pin setup (left & right motors) ===
const int motorL1 = 9;    // Left motor forward
const int motorL2 = 10;   // Left motor backward
const int motorR1 = 5;    // Right motor forward
const int motorR2 = 6;    // Right motor backward

// === Ultrasonic sensor pins ===
const int trigPin = 7;    // Sends signal out
const int echoPin = 8;    // Listens for echo

// === Line-following IR sensor pins ===
const int irLeft  = 2;    // Left IR sensor
const int irRight = 3;    // Right IR sensor

// === Distance and time thresholds ===
const int obstacleThreshold = 30;      // in cm — anything closer than this is considered an obstacle
const long recoveryTimeout  = 3000;    // how long to search for the line before giving up (ms)
const long avoidTimeout     = 5000;    // max time to try avoiding an obstacle (ms)

void setup() {
  // Set motor pins to OUTPUT mode so we can control them
  pinMode(motorL1, OUTPUT);
  pinMode(motorL2, OUTPUT);
  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);

  // Ultrasonic sensor setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // IR sensors just read input (no signals sent out)
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);

  // Start the Serial monitor so we can see debug prints
  Serial.begin(9600);
}

void loop() {
  long dist = getDistance();               // how far is the nearest object?
  bool irL = digitalRead(irLeft);          // is the left sensor seeing white?
  bool irR = digitalRead(irRight);         // is the right sensor seeing white?

  printStatus(dist, irL, irR);             // just for debugging

  if (dist < obstacleThreshold) {
    Serial.println("⚠️ Obstacle ahead! Trying to avoid it.");
    avoid();                               // try going around the obstacle
    return;
  }

  // Both sensors on black — this means we're following the line correctly
  if (!irL && !irR) {
    Serial.println("✅ On track. Moving forward.");
    moveForward();
  }

  // Left on black, right on white — drifting right, needs to turn left
  else if (!irL && irR) {
    Serial.println("↪ Need to adjust left.");
    turnLeft();
  }

  // Right on black, left on white — drifting left, needs to turn right
  else if (irL && !irR) {
    Serial.println("↩ Need to adjust right.");
    turnRight();
  }

  // Both sensors see white — probably lost the line, try to find it again
  else {
    Serial.println("❓ Lost the line... searching.");
    recover();
  }

  delay(100); // a short pause to make things smoother
}

// Sends a short pulse and listens for the echo.
// Returns distance in cm.
long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 20000); // listen for max 20ms
  if (duration == 0) return 999;                 // no echo? assume very far

  return duration * 0.034 / 2;                   // convert time to distance
}

// Print current sensor status to Serial (for debugging)
void printStatus(long dist, bool left, bool right) {
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.println(" cm");

  Serial.print("IR Left: ");
  Serial.print(left ? "WHITE" : "BLACK");
  Serial.print(" | IR Right: ");
  Serial.println(right ? "WHITE" : "BLACK");
}

// Move straight ahead
void moveForward() {
  digitalWrite(motorL1, HIGH);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, HIGH);
  digitalWrite(motorR2, LOW);
}

// Stops all movement
void stopMotors() {
  digitalWrite(motorL1, LOW);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, LOW);
}

// Turn left in place
void turnLeft() {
  digitalWrite(motorL1, LOW);
  digitalWrite(motorL2, HIGH);
  digitalWrite(motorR1, HIGH);
  digitalWrite(motorR2, LOW);
}

// Turn right in place
void turnRight() {
  digitalWrite(motorL1, HIGH);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, HIGH);
}

// Spin around until the IR sensors see the line again
void recover() {
  stopMotors();
  delay(100); // short pause before spinning

  // Spin in place clockwise
  digitalWrite(motorL1, HIGH);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, HIGH);

  unsigned long startTime = millis();

  // Keep spinning until either sensor sees black
  while (digitalRead(irLeft) == HIGH && digitalRead(irRight) == HIGH) {
    if (millis() - startTime > recoveryTimeout) {
      Serial.println("❌ Couldn't find the line. Stopping.");
      stopMotors();
      return;
    }
    delay(20); // don't check too fast
  }

  stopMotors();
  Serial.println("✅ Line recovered!");
}

// Try to go around an obstacle
void avoid() {
  unsigned long start = millis();

  turnRight();   // step 1: turn away
  delay(400);

  moveForward(); // step 2: go around
  unsigned long fwdStart = millis();
  while (millis() - fwdStart < 1200) {
    if (getDistance() > obstacleThreshold + 10) break;
    delay(50);
  }

  turnLeft();    // step 3: face forward again
  delay(400);

  moveForward(); // step 4: move back toward line
  delay(500);

  recover();     // step 5: re-align on the line

  if (millis() - start > avoidTimeout) {
    Serial.println("❌ Couldn't fully avoid. Proceeding anyway.");
    moveForward();
    delay(1000);
  }
}
