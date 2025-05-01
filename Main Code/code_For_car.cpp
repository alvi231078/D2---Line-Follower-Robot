// ==== Pin Assignments ====

const int MOTOR_L_FWD  = 9;
const int MOTOR_L_BWD  = 10;
const int MOTOR_R_FWD  = 5;
const int MOTOR_R_BWD  = 6;

const int TRIG_PIN     = 7;
const int ECHO_PIN     = 8;

const int IR_LEFT      = 2;
const int IR_RIGHT     = 3;

// ==== Config Constants ====

const int    OBSTACLE_DIST_CM = 30;
const long   MAX_RECOVERY_MS  = 3000;
const long   MAX_AVOID_MS     = 5000;

// ==== Arduino Setup ====

void setup() {
  pinMode(MOTOR_L_FWD, OUTPUT);
  pinMode(MOTOR_L_BWD, OUTPUT);
  pinMode(MOTOR_R_FWD, OUTPUT);
  pinMode(MOTOR_R_BWD, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  Serial.begin(9600);
}

// ==== Main Loop ====

void loop() {
  long dist = readDistance();
  bool irLeft  = digitalRead(IR_LEFT);
  bool irRight = digitalRead(IR_RIGHT);

  reportSensors(dist, irLeft, irRight);

  if      (dist < OBSTACLE_DIST_CM)     { Serial.println("⚠️ Obstacle! Avoiding..."); avoidObstacle(); return; }
  else if (!irLeft && !irRight)         { Serial.println("✅ Path clear. Forward.");   moveForward(); }
  else if (!irLeft && irRight)          { Serial.println("↪ Adjust left.");           turnLeft();    }
  else if (irLeft && !irRight)          { Serial.println("↩ Adjust right.");          turnRight();   }
  else                                  { Serial.println("❓ Searching for line...");   recoverLine(); }

  delay(100);
}

// ==== Distance Sensor ====

long readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 20000);
  return duration == 0 ? 999 : duration * 0.034 / 2;
}

// ==== Diagnostics ====

void reportSensors(long dist, bool left, bool right) {
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.println(" cm");

  Serial.print("IR Left: ");
  Serial.print(left ? "WHITE" : "BLACK");
  Serial.print(" | IR Right: ");
  Serial.println(right ? "WHITE" : "BLACK");
}

// ==== Navigation ====

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

// ==== Obstacle Avoidance ====

void avoidObstacle() {
  unsigned long start = millis();

  turnRight();
  delay(400);

  moveForward();
  unsigned long forwardStart = millis();
  while (millis() - forwardStart < 1200) {
    if (readDistance() > OBSTACLE_DIST_CM + 10) break;
    delay(50);
  }

  turnLeft();
  delay(400);

  moveForward();
  delay(500);

  recoverLine();

  if (millis() - start > MAX_AVOID_MS) {
    Serial.println("❌ Timeout. Moving ahead anyway.");
    moveForward();
    delay(1000);
  }
}

// ==== Line Recovery ====

void recoverLine() {
  stopMotors();
  delay(100);

  digitalWrite(MOTOR_L_FWD, HIGH);
  digitalWrite(MOTOR_L_BWD, LOW);
  digitalWrite(MOTOR_R_FWD, LOW);
  digitalWrite(MOTOR_R_BWD, HIGH);

  unsigned long recoveryStart = millis();

  while (digitalRead(IR_LEFT) == HIGH && digitalRead(IR_RIGHT) == HIGH) {
    if (millis() - recoveryStart > MAX_RECOVERY_MS) {
      Serial.println("❌ Line not found. Giving up.");
      stopMotors();
      return;
    }
    delay(20);
  }

  stopMotors();
  Serial.println("✅ Line recovered.");
}
