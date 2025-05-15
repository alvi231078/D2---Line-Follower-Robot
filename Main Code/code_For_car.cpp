// Motor driver pins
const int ENA = 10;  // PWM pin for Motor A speed
const int ENB = 11;  // PWM pin for Motor B speed
const int IN1 = 5;   // Motor A input 1
const int IN2 = 6;   // Motor A input 2
const int IN3 = 7;   // Motor B input 1
const int IN4 = 8;   // Motor B input 2

// IR sensor pins
const int leftSensorPin = 3;   // Digital IR sensor left
const int rightSensorPin = 4;  // Digital IR sensor right

void setup() {
  // Motor pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // IR sensors as inputs
  pinMode(leftSensorPin, INPUT);
  pinMode(rightSensorPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  int leftSensor = digitalRead(leftSensorPin);
  int rightSensor = digitalRead(rightSensorPin);

  Serial.print("Left IR: ");
  Serial.print(leftSensor);
  Serial.print(" | Right IR: ");
  Serial.println(rightSensor);

  if (leftSensor == 0 && rightSensor == 0) {
    moveForward();
  } 
  else if (leftSensor == 1 && rightSensor == 0) {
    turnLeft();
  } 
  else if (leftSensor == 0 && rightSensor == 1) {
    turnRight();
  } 
  else if (leftSensor == 1 && rightSensor == 1) {
    moveForward();  // or stopMotors(); if you want to stop when both sensors see black
  }
}

// Motor control functions with reduced speeds
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 150);  // Slower speed for accuracy
  analogWrite(ENB, 150);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
