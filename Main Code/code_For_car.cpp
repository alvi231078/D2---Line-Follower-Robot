// =====================
// === PIN DEFINITIONS ==
// =====================

#define MOTOR_L_FWD      9
#define MOTOR_L_BWD      10
#define MOTOR_R_FWD      5
#define MOTOR_R_BWD      6

#define TRIG_PIN         7
#define ECHO_PIN         8

#define IR_SENSOR_LEFT   2
#define IR_SENSOR_RIGHT  3

// =====================
// === CONFIGURATION ====
// =====================

#define OBSTACLE_LIMIT_CM      30
#define RECOVERY_TIMEOUT_MS    3000
#define AVOID_TIMEOUT_MS       5000

// =====================
// === SETUP ============
// =====================

void setup() {
  pinMode(MOTOR_L_FWD, OUTPUT);
  pinMode(MOTOR_L_BWD, OUTPUT);
  pinMode(MOTOR_R_FWD, OUTPUT);
  pinMode(MOTOR_R_BWD, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(IR_SENSOR_LEFT, INPUT);
  pinMode(IR_SENSOR_RIGHT, INPUT);

  Serial.begin(9600);
}

// =====================
// === MAIN LOOP ========
// =====================

void loop() {
  long dist = readDistance();
  bool leftIR  = digitalRead(IR_SENSOR_LEFT);
  bool rightIR = digitalRead(IR_SENSOR_RIGHT);

  displayStatus(dist, leftIR, rightIR);

  if      (dist < OBSTACLE_LIMIT_CM)      { Serial.println("⚠️ Obstacle ahead."); avoidObstacle(); return; }
  else if (!leftIR && !rightIR)           { Serial.println("✅ Forward.");        driveForward(); }
  else if (!leftIR &&  rightIR)           { Serial.println("↪ Left adjust.");     turnLeft();     }
  else if ( leftIR && !rightIR)           { Serial.println("↩ Right adjust.");    turnRight();    }
  else                                    { Serial.println("❓ Searching line.");   recoverLine();  }

  delay(100);
}

// =====================
// === SENSOR LOGIC =====
// =====================

long readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 20000);
  if (duration == 0) return 999;

  return duration * 0.034 / 2;
}

void displayStatus(long dist, bool left, bool right) {
  Serial.print("Distance: "); Serial.print(dist); Serial.println(" cm");

  Serial.print("IR Left: "); Serial.print(left ? "WHITE" : "BLACK");
  Serial.print(" | IR Right: "); Serial.println(right ? "WHITE" : "BLACK");
}

// =====================
// === MOTOR ACTIONS ====
// =====================

void driveForward() {
  digitalWrite(MOTOR_L_FWD, HIGH);  digitalWrite(MOTOR_L_BWD, LOW);
  digitalWrite(MOTOR_R_FWD, HIGH);  digitalWrite(MOTOR_R_BWD, LOW);
}

void stopMotors() {
  digitalWrite(MOTOR_L_FWD, LOW);   digitalWrite(MOTOR_L_BWD, LOW);
  digitalWrite(MOTOR_R_FWD, LOW);   digitalWrite(MOTOR_R_BWD, LOW);
}

void turnLeft() {
  digitalWrite(MOTOR_L_FWD, LOW);   digitalWrite(MOTOR_L_BWD, HIGH);
  digitalWrite(MOTOR_R_FWD, HIGH);  digitalWrite(MOTOR_R_BWD, LOW);
}

void turnRight() {
  digitalWrite(MOTOR_L_FWD, HIGH);  digitalWrite(MOTOR_L_BWD, LOW);
  digitalWrite(MOTOR_R_FWD, LOW);   digitalWrite(MOTOR_R_BWD, HIGH);
}

// =====================
// === OBSTACLE AVOIDANCE ==
// =====================

void avoidObstacle() {
  unsigned long startTime = millis();

  turnRight();   delay(400);
  driveForward();

  unsigned long moveTime = millis();
  while (millis() - moveTime < 1200) {
    if (readDistance() > OBSTACLE_LIMIT_CM + 10) break;
    delay(50);
  }

  turnLeft();    delay(400);
  driveForward(); delay(500);

  recoverLine();

  if (millis() - startTime > AVOID_TIMEOUT_MS) {
    Serial.println("❌ Avoid failed. Proceeding anyway.");
    driveForward(); delay(1000);
  }
}

// =====================
// === LINE RECOVERY =====
// =====================

void recoverLine() {
  stopMotors();
  delay(100);

  digitalWrite(MOTOR_L_FWD, HIGH);  digitalWrite(MOTOR_L_BWD, LOW);
  digitalWrite(MOTOR_R_FWD, LOW);   digitalWrite(MOTOR_R_BWD, HIGH);

  unsigned long startSpin = millis();

  while (digitalRead(IR_SENSOR_LEFT) == HIGH && digitalRead(IR_SENSOR_RIGHT) == HIGH) {
    if (millis() - startSpin > RECOVERY_TIMEOUT_MS) {
      Serial.println("❌ Line not found.");
      stopMotors();
      return;
    }
    delay(20);
  }

  stopMotors();
  Serial.println("✅ Line found.");
}
