#include <Servo.h> // Add Servo library

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

// === Servo Motor ===
const int servoPin = 11;
Servo myServo; // Create servo object

// === Thresholds and Timers ===
const int obstacleThreshold = 30;
const long recoveryTimeout = 3000;
const long avoidTimeout = 5000;
const unsigned long stuckTimeout = 4000;

unsigned long lastMoveTime = 0;
bool wasMoving = false;
int ultrasonicFailureCount = 0;
int maxSensorFailures = 3;

void setup() {
  pinMode(motorL1, OUTPUT); pinMode(motorL2, OUTPUT);
  pinMode(motorR1, OUTPUT); pinMode(motorR2, OUTPUT);

  pinMode(irLeft, INPUT); pinMode(irRight, INPUT);
  pinMode(trigPin, OUTPUT); pinMode(echoPin, INPUT);

  pinMode(S0, OUTPUT); pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT); pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  digitalWrite(S0, HIGH); digitalWrite(S1, LOW); // 20% scaling

  myServo.attach(servoPin); // Attach the servo
  myServo.write(90);        // Center position

  Serial.begin(9600);
}

void loop() {
  long distance = getDistanceCM();
  if (distance == -1) {
    ultrasonicFailureCount++;
    Serial.println("[ERROR] Ultrasonic sensor failed to respond.");
    if (ultrasonicFailureCount >= maxSensorFailures) {
      Serial.println("[CRITICAL] Too many ultrasonic failures. Stopping.");
      stopMotors();
      while (true);  // Hard stop
    }
    delay(200);
    return;
  } else {
    ultrasonicFailureCount = 0;
  }

  bool leftIR = isLineUnderSensor(irLeft);
  bool rightIR = isLineUnderSensor(irRight);
  String color = readColor();

  logStatus(distance, leftIR, rightIR, color);
  checkIfStuck(distance, leftIR, rightIR);

  // Color-based logic
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

  // Obstacle avoidance
  if (distance < obstacleThreshold) {
    Serial.println("[!] Obstacle detected. Avoiding...");

    // Sweep servo to scan for open direction
    for (int angle = 60; angle <= 120; angle += 15) {
      myServo.write(angle);
      delay(200);
      long scanDist = getDistanceCM();
      Serial.print("[Servo Scan] Angle: "); Serial.print(angle);
      Serial.print(" Distance: "); Serial.println(scanDist);
    }
    myServo.write(90); // Re-center
    avoidObstacle();
    return;
  }

  // Line following logic
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
    Serial.println("[X] Line lost. Attempting recovery...");
    recoverLine();
  }

  delay(100);
}
