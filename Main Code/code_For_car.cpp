// ========== Pin Definitions ==========

// Motor control pins
const int MOTOR_LEFT_FORWARD  = 9;
const int MOTOR_LEFT_BACKWARD = 10;
const int MOTOR_RIGHT_FORWARD = 5;
const int MOTOR_RIGHT_BACKWARD = 6;

// Ultrasonic sensor pins
const int ULTRASONIC_TRIG = 7;
const int ULTRASONIC_ECHO = 8;

// IR sensor pins
const int IR_SENSOR_LEFT  = 2;
const int IR_SENSOR_RIGHT = 3;


const int DISTANCE_THRESHOLD_CM = 30;
const unsigned long MAX_RECOVERY_TIME = 3000;  // 3 seconds
const unsigned long MAX_AVOID_TIME = 5000;     // 5 seconds



void setup() {
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);

  pinMode(ULTRASONIC_TRIG, OUTPUT);
  pinMode(ULTRASONIC_ECHO, INPUT);

  pinMode(IR_SENSOR_LEFT, INPUT);
  pinMode(IR_SENSOR_RIGHT, INPUT);

  Serial.begin(9600);
}

// ========== Main Loop ==========

void loop() {
  long distance = readUltrasonicDistance();
  bool isLeftWhite  = digitalRead(IR_SENSOR_LEFT);
  bool isRightWhite = digitalRead(IR_SENSOR_RIGHT);

  printSensorStatus(distance, isLeftWhite, isRightWhite);

  if (distance < DISTANCE_THRESHOLD_CM) {
    Serial.println("⚠️ Obstacle too close! Trying to avoid...");
    avoidObstacle();
    return;
  }

  if (!isLeftWhite && !isRightWhite) {
    Serial.println("✅ Path is clear — moving forward.");
    moveForward();
  }
  else if (!isLeftWhite && isRightWhite) {
    Serial.println("↪ Adjusting — turning left.");
    turnLeft();
  }
  else if (isLeftWhite && !isRightWhite) {
    Serial.println("↩ Adjusting — turning right.");
    turnRight();
  }
  else {
    Serial.println("❓ Line lost — recovering...");
    recoverLine();
  }

  delay(100); // small delay to stabilize reading
}

// ========== Sensor Functions ==========

long readUltrasonicDistance() {
  digitalWrite(ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG, LOW);

  long duration = pulseIn(ULTRASONIC_ECHO, HIGH, 20000);  // 20 ms timeout
  if (duration == 0) return 999;                          // if no signal return max
  return duration * 0.034 / 2;                            // convert to cm
}

void printSensorStatus(long distance, bool left, bool right) {
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("IR Left: ");
  Serial.print(left ? "WHITE" : "BLACK");
  Serial.print(" | IR Right: ");
  Serial.println(right ? "WHITE" : "BLACK");
}

// ========== Motor Control Functions ==========

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

// ========== Behavior Logic ==========

void avoidObstacle() {
  unsigned long avoidStart = millis();

  turnRight();
  delay(400);  // quick rotation

  moveForward();
  unsigned long forwardStart = millis();
  while (millis() - forwardStart < 1200) {
    if (readUltrasonicDistance() > DISTANCE_THRESHOLD_CM + 10) break;
    delay(50);
  }

  turnLeft();
  delay(400);  // face forward

  moveForward();
  delay(500);  // final alignment

  recoverLine();  // try to get back on track

  if (millis() - avoidStart > MAX_AVOID_TIME) {
    Serial.println("❌ Couldn’t avoid in time — forcing forward.");
    moveForward();
    delay(1000);
  }
}

void recoverLine() {
  stopMotors();
  delay(100);

  // Spin clockwise
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);

  unsigned long spinStart = millis();
  while (digitalRead(IR_SENSOR_LEFT) == HIGH && digitalRead(IR_SENSOR_RIGHT) == HIGH) {
    if (millis() - spinStart > MAX_RECOVERY_TIME) {
      Serial.println("❌ Gave up recovering the line.");
      stopMotors();
      return;
    }
    delay(20);
  }

  stopMotors();
  Serial.println("✅ Line found again.");
}
