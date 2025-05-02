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

// === Thresholds ===
const int obstacleThreshold = 30; // cm
const long recoveryTimeout = 3000;
const long avoidTimeout = 5000;

// === Setup ===
void setup() {
  // Motors
  pinMode(motorL1, OUTPUT); pinMode(motorL2, OUTPUT);
  pinMode(motorR1, OUTPUT); pinMode(motorR2, OUTPUT);

  // IR
  pinMode(irLeft, INPUT); pinMode(irRight, INPUT);

  // Ultrasonic
  pinMode(trigPin, OUTPUT); pinMode(echoPin, INPUT);

  // Color sensor pins
  pinMode(S0, OUTPUT); pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT); pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  // Set color sensor frequency scaling to 20%
  digitalWrite(S0, HIGH); digitalWrite(S1, LOW);

  Serial.begin(9600);
}

// === Loop ===
void loop() {
  long distance = getDistanceCM();
  bool leftIR = isLineUnderSensor(irLeft);
  bool rightIR = isLineUnderSensor(irRight);
  String color = readColor();

  logStatus(distance, leftIR, rightIR, color);

  // Color-based actions (if sensor is connected)
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

  // Obstacle detection
  if (distance < obstacleThreshold) {
    Serial.println("[!] Obstacle detected. Avoiding...");
    avoidObstacle();
    return;
  }

  // Line following
  if (!leftIR && !rightIR) {
    Serial.println("[✓] On line. Moving forward.");
    moveForward();
  } else if (!leftIR && rightIR) {
    Serial.println("[→] Adjusting left.");
    turnLeft();
  } else if (leftIR && !rightIR) {
    Serial.println("[←] Adjusting right.");
    turnRight();
  } else {
    Serial.println("[X] Line lost. Recovering...");
    recoverLine();
  }

  delay(100);
}

// === Movement Functions ===
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

// === Recovery and Avoidance ===
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

  // Basic check if sensor disconnected or noisy
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
