#include "motor_module.h"
#include <Arduino.h>

void setupMotors() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void moveForward(int leftSpeed, int rightSpeed) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  analogWrite(ENA, leftSpeed);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENB, rightSpeed);
}

void moveBackward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  analogWrite(ENA, REVERSE_SPEED);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENB, REVERSE_SPEED);
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  analogWrite(ENA, TURN_SPEED + 10);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENB, REVERSE_SPEED);
}

void turnLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  analogWrite(ENA, REVERSE_SPEED);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENB, TURN_SPEED + 10);
}

void rotateRight(int speed) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed);
}

void rotateLeft(int speed) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void moveForwardWithBias(int baseSpeed, float leftBias, float rightBias) {
  int leftSpeed = baseSpeed * leftBias;
  int rightSpeed = baseSpeed * rightBias;
  moveForward(leftSpeed, rightSpeed);
}

void gentleTurnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  analogWrite(ENA, BASE_SPEED);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENB, BASE_SPEED * 0.3);
}

void gentleTurnLeft() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  analogWrite(ENA, BASE_SPEED * 0.3);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENB, BASE_SPEED);
}
