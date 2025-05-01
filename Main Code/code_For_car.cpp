// ==== Motor Pins (Left and Right Wheels) ====
#define MOTOR_LEFT_FORWARD  9
#define MOTOR_LEFT_BACKWARD 10
#define MOTOR_RIGHT_FORWARD 5
#define MOTOR_RIGHT_BACKWARD 6

// ==== Ultrasonic Sensor Pins ====
#define TRIG_PIN 7
#define ECHO_PIN 8

// ==== IR Sensors (Line Following) ====
#define IR_LEFT  2
#define IR_RIGHT 3

// ==== Thresholds and Timeouts ====
#define OBSTACLE_DISTANCE_THRESHOLD 30   // in cm
#define RECOVERY_TIMEOUT           3000  // 3 seconds
#define AVOID_TIMEOUT              5000  // 5 seconds

void setup() {
  // Motor Pins
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);

  // Ultrasonic Sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // IR Sensors
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  // Serial Monitor
  Serial.begin(9600);
}

void loop() {
  long distance = getDistance();
  bool leftIR = digitalRead(IR_LEFT);   // white = HIGH, black = LOW
  bool rightIR = digitalRead(IR_RIGHT);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("IR Left: ");
  Serial.print(leftIR ? "WHITE" : "BLACK");
  Serial.print(" | IR Right: ");
  Serial.println(rightIR ? "WHITE" : "BLACK");

  if (distance < OBSTACLE_DISTANCE_THRESHOLD) {
    Serial.println("⚠️ Obstacle detected!");
    avoidObstacle();
    return;
  }

  if (!leftIR && !rightIR) {
    Serial.println("✅ Clear path — Moving forward");
    moveForward();
  } else if (!leftIR && rightIR) {
    Serial.println("↪ Adjusting — Turning left");
    turnLeft();
  } else if (leftIR && !rightIR) {
    Serial.println("↩ Adjusting — Turning right");
    turnRight();
  } else {
    Serial.println("🔍 Lost line — Recovering...");
    recoverLine();
  }

  delay(100);  // Smoothing out loop
}

// ==== Get Distance from Ultrasonic Sensor ====
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 20000);  // max wait: 20ms
  if (duration == 0) return 999;  // No reading
  return duration * 0.034 / 2;    // Convert to cm
}

// ==== Basic Motor Controls ====
void moveForward() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

void stopMotors() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

void turnLeft() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

void turnRight() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
}

// ==== Obstacle Avoidance Logic ====
void avoidObstacle() {
  unsigned long start = millis();

  turnRight();
  delay(400);

  moveForward();
  unsigned long forwardStart = millis();
  while (millis() - forwardStart < 1200) {
    if (getDistance() > OBSTACLE_DISTANCE_THRESHOLD + 10) break;
    delay(50);
  }

  turnLeft();
  delay(400);

  moveForward();
  delay(500);

  recoverLine();

  if (millis() - start > AVOID_TIMEOUT) {
    Serial.println("❌ Obstacle avoid timeout — Moving forward anyway");
    moveForward();
    delay(1000);
  }
}

// ==== Line Recovery Logic ====
void recoverLine() {
  stopMotors();
  delay(100);

  // Spin in place (clockwise)
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);

  unsigned long startTime = millis();
  while (digitalRead(IR_LEFT) == HIGH && digitalRead(IR_RIGHT) == HIGH) {
    if (millis() - startTime > RECOVERY_TIMEOUT) {
      Serial.println("❌ Line not found — Giving up");
      stopMotors();
      return;
    }
    delay(20);
  }

  stopMotors();
  Serial.println("✅ Line found!");
}
