// === Includes ===
#include <Servo.h>

// === Motor Driver Pins ===
const int ENA = 10;
const int ENB = 11;
const int IN1 = 5;
const int IN2 = 6;
const int IN3 = 7;
const int IN4 = 8;

// === IR Sensor Pins ===
const int LEFT_SENSOR  = 3;
const int RIGHT_SENSOR = 4;

// === Speed Settings ===
const int BASE_SPEED    = 90;
const int TURN_SPEED    = 70;
const int REVERSE_SPEED = 70;

// === IR Sensor Logic ===
const bool LINE_DETECTED_STATE = false;
const int LOOP_DELAY = 8;
const int SENSOR_READ_INTERVAL = 2;
const int SENSOR_SAMPLES = 3;

// === Ultrasonic Sensor Pins ===
const int TRIG_PIN = 13;
const int ECHO_PIN = 12;
const int OBSTACLE_DISTANCE_THRESHOLD = 10; // cm

// === Servo Motor ===
Servo myServo;

// === Sensor Filtering Variables ===
unsigned long lastSensorRead = 0;
unsigned long lastDebugPrint = 0;
const int DEBUG_INTERVAL = 200;

bool leftOnLine = false, rightOnLine = false;
bool prevLeftOnLine = false, prevRightOnLine = false;

int leftSensorHistory[SENSOR_SAMPLES] = {0};
int rightSensorHistory[SENSOR_SAMPLES] = {0};
int sampleIndex = 0;

bool rightSensorReliable = true;
unsigned long rightSensorLastDetected = 0;

void setup() {
  // === Setup Motor Pins ===
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  // === Setup Sensors and Servo ===
  pinMode(LEFT_SENSOR, INPUT); pinMode(RIGHT_SENSOR, INPUT);
  pinMode(TRIG_PIN, OUTPUT); pinMode(ECHO_PIN, INPUT);
  myServo.attach(9);
  myServo.write(90); // Center position

  // === Serial Debugging ===
  Serial.begin(9600);
  while (!Serial); // Wait for Arduino UNO R4 WiFi
  Serial.println("LFR + Obstacle Avoidance Init");

  stopMotors();
  delay(1000);
}

void loop() {
  unsigned long currentTime = millis();

  // === Obstacle Check ===
  int distance = readDistance();
  if (distance > 0 && distance <= OBSTACLE_DISTANCE_THRESHOLD) {
    Serial.println("Obstacle <10cm - Stop, Reverse, Turn Servo Right");

    stopMotors();
    delay(300);

    reverse(REVERSE_SPEED);
    delay(500); // Move back

    stopMotors();
    myServo.write(150); // Look right
    delay(700);
    myServo.write(90);  // Reset to center
    delay(300);
    return; // Skip line following during obstacle
  }

  // === Sensor Reading and Line Following ===
  if (currentTime - lastSensorRead >= SENSOR_READ_INTERVAL) {
    readSensors();
    lastSensorRead = currentTime;
  }

  executeLineFollowing();

  // === Debug Output ===
  if (currentTime - lastDebugPrint >= DEBUG_INTERVAL) {
    printDebugInfo();
    lastDebugPrint = currentTime;
  }

  delay(LOOP_DELAY);
}

// === Read IR Sensors with Filtering ===
void readSensors() {
  prevLeftOnLine = leftOnLine;
  prevRightOnLine = rightOnLine;

  int leftTotal = 0, rightTotal = 0;
  for (int i = 0; i < 3; i++) {
    leftTotal += digitalRead(LEFT_SENSOR);
    rightTotal += digitalRead(RIGHT_SENSOR);
    delayMicroseconds(500);
  }

  bool leftRaw = (leftTotal >= 2);
  bool rightRaw = (rightTotal >= 2);

  leftSensorHistory[sampleIndex] = leftRaw;
  rightSensorHistory[sampleIndex] = rightRaw;

  int leftSum = 0, rightSum = 0;
  for (int i = 0; i < SENSOR_SAMPLES; i++) {
    leftSum += leftSensorHistory[i];
    rightSum += rightSensorHistory[i];
  }

  bool leftFiltered = (leftSum >= (SENSOR_SAMPLES / 2 + 1));
  bool rightFiltered = (rightSum >= (SENSOR_SAMPLES / 2 + 1));

  leftOnLine = (leftFiltered == LINE_DETECTED_STATE);
  rightOnLine = (rightFiltered == LINE_DETECTED_STATE);

  if (rightOnLine) {
    rightSensorLastDetected = millis();
    rightSensorReliable = true;
  } else if (millis() - rightSensorLastDetected > 5000) {
    rightSensorReliable = false;
  }

  sampleIndex = (sampleIndex + 1) % SENSOR_SAMPLES;
}

// === Line Following Motor Control ===
void executeLineFollowing() {
  if (!leftOnLine && !rightOnLine) {
    moveForward(BASE_SPEED, BASE_SPEED);
  } else if (leftOnLine && !rightOnLine) {
    if (!rightSensorReliable) gentleTurnRight();
    else turnRight();
  } else if (!leftOnLine && rightOnLine) {
    turnLeft();
  } else {
    moveForward(BASE_SPEED * 0.6, BASE_SPEED * 0.6);
  }

  if (!rightSensorReliable && leftOnLine) {
    moveForwardWithBias(BASE_SPEED, 0.8, 1.0);
  }
}

// === Motion Functions ===
void moveForward(int l, int r) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, l);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, r);
}

void reverse(int speed) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); analogWrite(ENA, speed);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); analogWrite(ENB, speed);
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, TURN_SPEED + 10);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); analogWrite(ENB, REVERSE_SPEED);
}

void turnLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); analogWrite(ENA, REVERSE_SPEED);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, TURN_SPEED + 10);
}

void gentleTurnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, BASE_SPEED);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, BASE_SPEED * 0.3);
}

void moveForwardWithBias(int base, float lBias, float rBias) {
  int l = base * lBias;
  int r = base * rBias;
  moveForward(l, r);
}

// === Serial Debug Info ===
void printDebugInfo() {
  Serial.print("L:"); Serial.print(leftOnLine ? "■" : "□");
  Serial.print(" R:"); Serial.print(rightOnLine ? "■" : "□");
  if (!rightSensorReliable) Serial.print(" [R-FAULT]");
  Serial.print(" | ");
  if (!leftOnLine && !rightOnLine) Serial.println("FORWARD");
  else if (leftOnLine && !rightOnLine) Serial.println(rightSensorReliable ? "TURN RIGHT" : "GENTLE RIGHT");
  else if (!leftOnLine && rightOnLine) Serial.println("TURN LEFT");
  else Serial.println("BOTH ON LINE");
}

// === Ultrasonic Sensor Distance Reading ===
int readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2; // Convert to cm
}
