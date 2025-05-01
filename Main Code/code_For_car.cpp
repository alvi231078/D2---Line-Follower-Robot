// ==== Pin Configuration ====
const int MOTOR_LEFT_FWD     = 9;
const int MOTOR_LEFT_BWD     = 10;
const int MOTOR_RIGHT_FWD    = 5;
const int MOTOR_RIGHT_BWD    = 6;

const int ULTRASONIC_TRIG    = 7;
const int ULTRASONIC_ECHO    = 8;

const int IR_SENSOR_LEFT     = 2;
const int IR_SENSOR_RIGHT    = 3;

// ==== Constants ====
const int OBSTACLE_THRESHOLD_CM = 30;
const unsigned long RECOVERY_LIMIT_MS = 3000;
const unsigned long AVOID_LIMIT_MS    = 5000;

void setup() {
  // Motor setup
  pinMode(MOTOR_LEFT_FWD, OUTPUT);
  pinMode(MOTOR_LEFT_BWD, OUTPUT);
  pinMode(MOTOR_RIGHT_FWD, OUTPUT);
  pinMode(MOTOR_RIGHT_BWD, OUTPUT);

  // Sensor setup
  pinMode(ULTRASONIC_TRIG, OUTPUT);
  pinMode(ULTRASONIC_ECHO, INPUT);
  pinMode(IR_SENSOR_LEFT, INPUT);
  pinMode(IR_SENSOR_RIGHT, INPUT);

  Serial.begin(9600);
}

void loop() {
  long dist = measureDistance();
  bool leftDetected  = digitalRead(IR_SENSOR_LEFT);
  bool rightDetected = digitalRead(IR_SENSOR_RIGHT);

  printSensorData(dist, leftDetected, rightDetected);

  if (dist < OBSTACLE_THRESHOLD_CM) {
    Serial.println("⚠️ Obstacle detected.");
    avoidObstacle();
    return;
  }

  if (!leftDetected && !rightDetected) {
    Serial.println("✅ Forward movement.");
    moveForward();
  } else if (!leftDetected && rightDetected) {
    Serial.println("↪ Turning left.");
    turnLeft();
  } else if (leftDetected && !rightDetected) {
    Serial.println("↩ Turning right.");
    turnRight();
  } else {
    Serial.println("🌀 Line lost — recovering.");
    recoverLine();
  }

  delay(100);
}

// ==== Sensor Functions ====
long measureDistance() {
  digitalWrite(ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG, LOW);

  long duration = pulseIn(ULTRASONIC_ECHO, HIGH, 20000);
  return (duration == 0) ? 999 : duration * 0.034 / 2;
}

void printSensorData(long distance, bool left, bool right) {
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("IR Left: ");
  Serial.print(left ? "WHITE" : "BLACK");
  Serial.print(" | IR Right: ");
  Serial.println(right ? "WHITE" : "BLACK");
}

// ==== Motion Functions ====
void moveForward() {
  digitalWrite(MOTOR_LEFT_FWD, HIGH);
  digitalWrite(MOTOR_LEFT_BWD, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, HIGH);
  digitalWrite(MOTOR_RIGHT_BWD, LOW);
}

void stopMotors() {
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_BWD, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, LOW);
  digitalWrite(MOTOR_RIGHT_BWD, LOW);
}

void turnLeft() {
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_BWD, HIGH);
  digitalWrite(MOTOR_RIGHT_FWD, HIGH);
  digitalWrite(MOTOR_RIGHT_BWD, LOW);
}

void turnRight() {
  digitalWrite(MOTOR_LEFT_FWD, HIGH);
  digitalWrite(MOTOR_LEFT_BWD, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, LOW);
  digitalWrite(MOTOR_RIGHT_BWD, HIGH);
}

// ==== Behavior Functions ====
void avoidObstacle() {
  unsigned long start = millis();
//  Serial.println("🚧 Avoiding obstacle.");
  turnRight();
  delay(400);

  moveForward();
  unsigned long forwardStart = millis();
  while (millis() - forwardStart < 1200) {
    if (measureDistance() > OBSTACLE_THRESHOLD_CM + 10) break;
    delay(50);
  }

  turnLeft();
  delay(400);

  moveForward();
  delay(500);
//  Serial.println("🔄 Attempting to recover line.");
  recoverLine();

  if (millis() - start > AVOID_LIMIT_MS) {
    Serial.println("❌ Timeout — forcing forward.");
    moveForward();
    delay(1000);
  }
}

void recoverLine() {
  stopMotors();
  delay(100);
  // Serial.println("🌀 Attempting to recover line.");
  // Spin in place
  digitalWrite(MOTOR_LEFT_FWD, HIGH);
  digitalWrite(MOTOR_LEFT_BWD, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, LOW);
  digitalWrite(MOTOR_RIGHT_BWD, HIGH);

  unsigned long recoveryStart = millis();
  while (digitalRead(IR_SENSOR_LEFT) == HIGH && digitalRead(IR_SENSOR_RIGHT) == HIGH) {
    if (millis() - recoveryStart > RECOVERY_LIMIT_MS) {
      Serial.println("❌ Recovery failed.");
      stopMotors();
      return;
    }
    delay(20);
  }

  stopMotors();
  Serial.println("✅ Line recovered.");
}
