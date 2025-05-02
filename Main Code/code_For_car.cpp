// === Motor Pins ===
const int motorL1 = 9;
const int motorL2 = 10;
const int motorR1 = 5;
const int motorR2 = 6;
const int motorSpeed = 150;

// === IR Sensors ===
const int irLeft = 2;
const int irRight = 3;

// === Ultrasonic Sensor ===
const int trigPin = 7;
const int echoPin = 8;

// === TCS3200 Color Sensor Pins (optional) ===
const int S0 = 4;
const int S1 = A0;
const int S2 = A1;
const int S3 = A2;
const int sensorOut = A3;

// === Thresholds and Timers ===
const int obstacleThreshold = 30; // cm
const long recoveryTimeout = 3000;
const long avoidTimeout = 5000;
const unsigned long stuckTimeout = 4000; // ms

// === State Tracking ===
unsigned long lastMoveTime = 0;
bool wasMoving = false;

void setup() {
  pinMode(motorL1, OUTPUT); pinMode(motorL2, OUTPUT);
  pinMode(motorR1, OUTPUT); pinMode(motorR2, OUTPUT);

  pinMode(irLeft, INPUT); pinMode(irRight, INPUT);

  pinMode(trigPin, OUTPUT); pinMode(echoPin, INPUT);

  pinMode(S0, OUTPUT); pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT); pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  digitalWrite(S0, HIGH); digitalWrite(S1, LOW); // 20% scaling

  Serial.begin(9600);
}

void loop() {
  long distance = getDistanceCM();
  bool leftIR = isLineUnderSensor(irLeft);
  bool rightIR = isLineUnderSensor(irRight);
  String color = readColor();

  logStatus(distance, leftIR, rightIR, color);

  checkIfStuck(distance, leftIR, rightIR);

  if (color == "RED") {
    Serial.println("[COLOR] RED → Stopping");
    stopMotors();
    delay(2000);
    return;
  } else if (color == "GREEN") {
    Serial.println("[COLOR] GREEN → Boosting forward");
    moveForward();
    delay(800);
    return;
  } else if (color == "BLUE") {
    Serial.println("[COLOR] BLUE → Turning right");
    turnRight();
    delay(500);
    return;
  }

  if (distance < obstacleThreshold) {
    Serial.println("[!] Obstacle detected. Avoiding...");
    avoidObstacle();
    return;
  }

  if (!leftIR && !rightIR) {
    Serial.println("[✓] On line. Moving forward.");
    moveForward();
    wasMoving = true;
    lastMoveTime = millis();
  } else if (!leftIR && rightIR) {
    Serial.println("[→] Adjusting left.");
    turnLeft();
    wasMoving = true;
    lastMoveTime = millis();
  } else if (leftIR && !rightIR) {
    Serial.println("[←] Adjusting right.");
    turnRight();
    wasMoving = true;
    lastMoveTime = millis();
  } else {
    Serial.println("[X] Line lost. Recovering...");
    recoverLine();
  }

  delay(100);
}

// === Self-Retraction if Stuck ===
void checkIfStuck(long distance, bool leftIR, bool rightIR) {
  static long lastDistance = 0;

  if (wasMoving && !leftIR && !rightIR && abs(distance - lastDistance) < 3) {
    if (millis() - lastMoveTime > stuckTimeout) {
      Serial.println("[STUCK] No progress detected. Reversing...");

      // Reverse motion
      digitalWrite(motorL1, LOW);
      analogWrite(motorL2, motorSpeed);
      digitalWrite(motorR1, LOW);
      analogWrite(motorR2, motorSpeed);
      delay(1000);

      stopMotors();
      delay(300);

      recoverLine();

      wasMoving = false;
      lastMoveTime = millis();
    }
  }

  lastDistance = distance;
}

// === Movement ===
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

// === Recovery ===
void recoverLine() {
  stopMotors();
  delay(100);

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
  Serial.println("[✓] Line recovered.");
}

// === Obstacle Avoidance ===
void avoidObstacle() {
  unsigned long start = millis();

  turnRight();
  delay(400);

  moveForward();
  unsigned long forwardStart = millis();
  while (millis() - forwardStart < 1200) {
    if (getDistanceCM() > obstacleThreshold + 10) break;
    delay(50);
  }

  turnLeft();
  delay(400);

  moveForward();
  delay(500);

  recoverLine();

  if (millis() - start > avoidTimeout) {
    Serial.println("[!] Avoidance timeout. Forcing forward.");
    moveForward();
    delay(1000);
  }
}

// === Sensor Functions ===
long getDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 20000);
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

bool isLineUnderSensor(int pin) {
  return digitalRead(pin) == LOW;
}

// === Color Sensor ===
String readColor() {
  int red = readColorFrequency(LOW, LOW);
  int green = readColorFrequency(HIGH, HIGH);
  int blue = readColorFrequency(LOW, HIGH);

  if (red > 3000 && green > 3000 && blue > 3000) return "UNKNOWN";

  Serial.print("R: "); Serial.print(red);
  Serial.print(" G: "); Serial.print(green);
  Serial.print(" B: "); Serial.println(blue);

  if (red < green && red < blue) return "RED";
  if (green < red && green < blue) return "GREEN";
  if (blue < red && blue < green) return "BLUE";

  return "UNKNOWN";
}

int readColorFrequency(bool s2Val, bool s3Val) {
  digitalWrite(S2, s2Val);
  digitalWrite(S3, s3Val);
  delay(50);
  return pulseIn(sensorOut, LOW);
}

// === Logging ===
void logStatus(long dist, bool left, bool right, String color) {
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.print(" cm | IR L: ");
  Serial.print(left ? "BLACK" : "WHITE");
  Serial.print(" | IR R: ");
  Serial.print(right ? "BLACK" : "WHITE");
  Serial.print(" | Color: ");
  Serial.println(color);
}
