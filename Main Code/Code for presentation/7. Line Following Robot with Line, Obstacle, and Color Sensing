// === Includes and Constants ===
#include <Servo.h>

// Motor driver pins
const int ENA = 10, ENB = 11;
const int IN1 = 5, IN2 = 6, IN3 = 7, IN4 = 8;

// IR sensor pins
const int LEFT_SENSOR = 3, RIGHT_SENSOR = 4;

// Speed settings
const int BASE_SPEED = 90, TURN_SPEED = 70, REVERSE_SPEED = 70;

// IR logic & control
const bool LINE_DETECTED_STATE = false;
const int LOOP_DELAY = 8, SENSOR_READ_INTERVAL = 2, SENSOR_SAMPLES = 3;

// Ultrasonic & threshold
const int TRIG_PIN = 13, ECHO_PIN = 12, OBSTACLE_DISTANCE_THRESHOLD = 10;

// Color sensor pins
const int COLOR_S0 = A0;
const int COLOR_S1 = A1;
const int COLOR_S2 = A2;
const int COLOR_S3 = A3;
const int COLOR_OUT = A4;
const int COLOR_LED = A5;

// Battery Voltage Pin (assumed pin A6 for battery voltage monitoring)
const int BATTERY_PIN = A6;
const float VOLTAGE_THRESHOLD = 4.0;  // 4V threshold for motor speed adjustment

Servo myServo;

unsigned long lastSensorRead = 0, lastDebugPrint = 0;
const int DEBUG_INTERVAL = 200;
bool leftOnLine = false, rightOnLine = false;
bool rightSensorReliable = true;
unsigned long rightSensorLastDetected = 0;
int leftSensorHistory[SENSOR_SAMPLES] = {}, rightSensorHistory[SENSOR_SAMPLES] = {}, sampleIndex = 0;

int redFrequency = 0, greenFrequency = 0, blueFrequency = 0;

void setup() {
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(LEFT_SENSOR, INPUT); pinMode(RIGHT_SENSOR, INPUT);
  pinMode(TRIG_PIN, OUTPUT); pinMode(ECHO_PIN, INPUT);

  pinMode(COLOR_S0, OUTPUT); pinMode(COLOR_S1, OUTPUT);
  pinMode(COLOR_S2, OUTPUT); pinMode(COLOR_S3, OUTPUT);
  pinMode(COLOR_LED, OUTPUT); pinMode(COLOR_OUT, INPUT);
  digitalWrite(COLOR_S0, HIGH); digitalWrite(COLOR_S1, LOW);
  digitalWrite(COLOR_LED, HIGH);

  myServo.attach(9); myServo.write(90);
  Serial.begin(9600); while (!Serial);
  Serial.println("LFR + Obstacle Avoidance + Color Detection Init");
  stopMotors(); delay(1000);
}

void loop() {
  unsigned long currentTime = millis();
  int distance = readDistance();

  // Read battery voltage and check if it's below 4V
  float batteryVoltage = readBatteryVoltage();
  int motorSpeed = (batteryVoltage < VOLTAGE_THRESHOLD) ? BASE_SPEED : BASE_SPEED;  // Default to BASE_SPEED if under threshold

  if (distance > 0 && distance <= OBSTACLE_DISTANCE_THRESHOLD) {
    stopMotors(); delay(300);
    readColor();
    String color = getDetectedColor();
    Serial.print("R:"); Serial.print(redFrequency);
    Serial.print(" G:"); Serial.print(greenFrequency);
    Serial.print(" B:"); Serial.print(blueFrequency);
    Serial.print(" | Distance: "); Serial.print(distance);
    Serial.print(" cm | Detected Color: "); Serial.println(color);

    if (color == "Green") {
      Serial.println("Action: PUSH GREEN OBJECT");
      pushObject();
      return;
    } else {
      Serial.println("Action: AVOID");
      reverse(REVERSE_SPEED); delay(300);
      stopMotors();

      myServo.write(150); delay(500); int distRight = readDistance();
      myServo.write(30); delay(500); int distLeft = readDistance();
      myServo.write(90); delay(300);

      if (distLeft > distRight) {
        turnLeft(); delay(700);
        moveForward(motorSpeed, motorSpeed); delay(600);
        turnRight(); delay(700);
        moveForward(motorSpeed, motorSpeed); delay(600);
        finalAlignRight(); delay(600); // reduced speed turn
      } else {
        turnRight(); delay(700);
        moveForward(motorSpeed, motorSpeed); delay(600);
        turnLeft(); delay(700);
        moveForward(motorSpeed, motorSpeed); delay(600);
        finalAlignLeft(); delay(600); // reduced speed turn
      }
      stopMotors();
      return;
    }
  }

  if (currentTime - lastSensorRead >= SENSOR_READ_INTERVAL) {
    readSensors(); lastSensorRead = currentTime;
  }

  executeLineFollowing();

  if (currentTime - lastDebugPrint >= DEBUG_INTERVAL) {
    printDebugInfo(); lastDebugPrint = currentTime;
  }

  delay(LOOP_DELAY);
}

// Function to read the battery voltage from the specified pin
float readBatteryVoltage() {
  int sensorValue = analogRead(BATTERY_PIN);
  float voltage = sensorValue * (5.0 / 1023.0) * 2;  // Assuming a voltage divider giving 2x amplification
  return voltage;
}

void readColor() {
  redFrequency = readColorFrequency(LOW, LOW);
  greenFrequency = readColorFrequency(HIGH, HIGH);
  blueFrequency = readColorFrequency(LOW, HIGH);
}

int readColorFrequency(bool s2, bool s3) {
  digitalWrite(COLOR_S2, s2);
  digitalWrite(COLOR_S3, s3);
  delay(100);
  return pulseIn(COLOR_OUT, LOW);
}

String getDetectedColor() {
  if (redFrequency < greenFrequency && redFrequency < blueFrequency) return "Red";
  if (greenFrequency < redFrequency && greenFrequency < blueFrequency) return "Green";
  if (blueFrequency < redFrequency && blueFrequency < greenFrequency) return "Blue";
  return "Unknown";
}

void readSensors() {
  int leftTotal = 0, rightTotal = 0;
  for (int i = 0; i < 3; i++) {
    leftTotal += digitalRead(LEFT_SENSOR);
    rightTotal += digitalRead(RIGHT_SENSOR);
    delayMicroseconds(500);
  }
  bool leftRaw = (leftTotal >= 2), rightRaw = (rightTotal >= 2);
  leftSensorHistory[sampleIndex] = leftRaw;
  rightSensorHistory[sampleIndex] = rightRaw;

  int leftSum = 0, rightSum = 0;
  for (int i = 0; i < SENSOR_SAMPLES; i++) {
    leftSum += leftSensorHistory[i]; rightSum += rightSensorHistory[i];
  }
  leftOnLine = (leftSum >= (SENSOR_SAMPLES / 2 + 1)) == LINE_DETECTED_STATE;
  rightOnLine = (rightSum >= (SENSOR_SAMPLES / 2 + 1)) == LINE_DETECTED_STATE;

  if (rightOnLine) {
    rightSensorLastDetected = millis(); rightSensorReliable = true;
  } else if (millis() - rightSensorLastDetected > 5000) {
    rightSensorReliable = false;
  }
  sampleIndex = (sampleIndex + 1) % SENSOR_SAMPLES;
}

void executeLineFollowing() {
  if (!leftOnLine && !rightOnLine) moveForward(BASE_SPEED, BASE_SPEED);
  else if (leftOnLine && !rightOnLine) rightSensorReliable ? turnRight() : gentleTurnRight();
  else if (!leftOnLine && rightOnLine) turnLeft();
  else moveForward(BASE_SPEED * 0.6, BASE_SPEED * 0.6);

  if (!rightSensorReliable && leftOnLine)
    moveForwardWithBias(BASE_SPEED, 0.8, 1.0);
}

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
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, TURN_SPEED);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); analogWrite(ENB, TURN_SPEED);
}

void turnLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); analogWrite(ENA, TURN_SPEED);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, TURN_SPEED);
}

void gentleTurnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, BASE_SPEED);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, BASE_SPEED * 0.3);
}

void moveForwardWithBias(int base, float lBias, float rBias) {
  moveForward(base * lBias, base * rBias);
}

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

int readDistance() {
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  return pulseIn(ECHO_PIN, HIGH) * 0.034 / 2;
}

void pushObject() {
  moveForward(BASE_SPEED, BASE_SPEED); delay(2000); // updated from 1000 to 2000
  stopMotors(); delay(500);
}

void finalAlignRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, 60);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); analogWrite(ENB, 60);
}

void finalAlignLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); analogWrite(ENA, 60);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, 60);
}
