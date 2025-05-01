// ==== Motor Pins ====
const int MOTOR_L_FWD  = 9;
const int MOTOR_L_BWD  = 10;
const int MOTOR_R_FWD  = 5;
const int MOTOR_R_BWD  = 6;

// ==== Ultrasonic Sensor ====
const int TRIG_PIN = 7;
const int ECHO_PIN = 8;

// ==== IR Sensors ====
const int IR_LEFT_PIN  = 2;
const int IR_RIGHT_PIN = 3;

// ==== Thresholds ====
const int DIST_THRESHOLD_CM   = 30;
const unsigned long RECOVERY_TIMEOUT_MS = 3000;
const unsigned long AVOID_TIMEOUT_MS    = 5000;

void setup() {
  // Motor setup
  pinMode(MOTOR_L_FWD, OUTPUT);
  pinMode(MOTOR_L_BWD, OUTPUT);
  pinMode(MOTOR_R_FWD, OUTPUT);
  pinMode(MOTOR_R_BWD, OUTPUT);

  // Sensor setup
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(IR_LEFT_PIN, INPUT);
  pinMode(IR_RIGHT_PIN, INPUT);

  Serial.begin(9600);
}

void loop() {
  long distance = readDistanceCM();
  bool leftLine  = digitalRead(IR_LEFT_PIN);
  bool rightLine = digitalRead(IR_RIGHT_PIN);

  logSensors(distance, leftLine, rightLine);

  if (distance < DIST_THRESHOLD_CM) {
    Serial.println("⚠️ Obstacle detected");
    avoidObstacle();
    return;
  }

  if (!leftLine && !rightLine) {
    Serial.println("✅ Moving forward");
    moveForward();
  } else if (!leftLine && rightLine) {
    Serial.println("↪ Adjusting left");
    turnLeft();
  } else if (leftLine && !rightLine) {
    Serial.println("↩ Adjusting right");
    turnRight();
  } else {
    Serial.println("❓ Lost line — recovering");
    recoverLine();
  }

  delay(100);
}

long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 20000);
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

void logSensors(long dist, bool left, bool right) {
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.println(" cm");

  Serial.print("IR Left: ");
  Serial.print(left ? "WHITE" : "BLACK");
  Serial.print(" | IR Right: ");
  Serial.println(right ? "WHITE" : "BLACK");
}

void moveForward() {
  digitalWrite(MOTOR_L_FWD, HIGH);
  digitalWrite(MOTOR_L_BWD, LOW);
  digitalWrite(MOTOR_R_FWD, HIGH);
  digitalWrite(MOTOR_R_BWD, LOW);
}

void stopMotors() {
  digitalWrite(MOTOR_L_FWD, LOW);
  digitalWrite(MOTOR_L_BWD, LOW);
  digitalWrite(MOTOR_R_FWD, LOW);
  digitalWrite(MOTOR_R_BWD, LOW);
}

void turnLeft() {
  digitalWrite(MOTOR_L_FWD, LOW);
  digitalWrite(MOTOR_L_BWD, HIGH);
  digitalWrite(MOTOR_R_FWD, HIGH);
  digitalWrite(MOTOR_R_BWD, LOW);
}

void turnRight() {
  digitalWrite(MOTOR_L_FWD, HIGH);
  digitalWrite(MOTOR_L_BWD, LOW);
  digitalWrite(MOTOR_R_FWD, LOW);
  digitalWrite(MOTOR_R_BWD, HIGH);
}

void avoidObstacle() {
  unsigned long startTime = millis();

  turnRight();
  delay(400);

  moveForward();
  unsigned long forwardStart = millis();
  while (millis() - forwardStart < 1200) {
    if (readDistanceCM() > DIST_THRESHOLD_CM + 10) break;
    delay(50);
  }

  turnLeft();
  delay(400);

  moveForward();
  delay(500);

  recoverLine();

  if (millis() - startTime > AVOID_TIMEOUT_MS) {
    Serial.println("❌ Timeout — moving forward anyway");
    moveForward();
    delay(1000);
  }
}

void recoverLine() {
  stopMotors();
  delay(100);

  // Spin in place (clockwise)
  digitalWrite(MOTOR_L_FWD, HIGH);
  digitalWrite(MOTOR_L_BWD, LOW);
  digitalWrite(MOTOR_R_FWD, LOW);
  digitalWrite(MOTOR_R_BWD, HIGH);

  unsigned long recoveryStart = millis();
  while (digitalRead(IR_LEFT_PIN) == HIGH && digitalRead(IR_RIGHT_PIN) == HIGH) {
    if (millis() - recoveryStart > RECOVERY_TIMEOUT_MS) {
      Serial.println("❌ Line recovery failed");
      stopMotors();
      return;
    }
    delay(20);
  }

  stopMotors();
  Serial.println("✅ Line recovered");
}
// This code is designed to control a line-following robot with obstacle avoidance capabilities.
// It uses two IR sensors to follow a line and an ultrasonic sensor to detect obstacles.