#include <Servo.h>

// Motor driver pins
const int ENA = 10;  // PWM Motor A (left)
const int IN1 = 5;   // Motor A input 1
const int IN2 = 6;   // Motor A input 2
const int ENB = 11;  // PWM Motor B (right)
const int IN3 = 7;   // Motor B input 1
const int IN4 = 8;   // Motor B input 2

// IR sensor pins (digital)
const int LEFT_SENSOR  = 3;
const int RIGHT_SENSOR = 4;

// Ultrasonic sensor pins
const int TRIG_PIN = 12;
const int ECHO_PIN = 13;

// Servo for future use (kept centered)
const int SERVO_PIN    = 9;
const int SERVO_CENTER = 67;
Servo scanner;

// Distance threshold for obstacle (cm)
const int OBSTACLE_THRESHOLD_CM = 6;

// Speed settings (PWM 0-255)
const int BASE_SPEED        = 70;   // Normal forward speed
const int TURN_SPEED        = 80;   // Speed during turns
const int BACKWARD_SPEED    = 60;   // Speed when moving backward

// Push mode speeds
const int PUSH_BASE_SPEED   = 150;  // Increased forward speed to push
const int PUSH_TURN_SPEED   = 160;  // Increased turn speed when pushing

void setup() {
  // Motor pins
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  
  // Sensors
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Servo centered
  scanner.attach(SERVO_PIN);
  scanner.write(SERVO_CENTER);

  Serial.begin(9600);
  Serial.println("Line follower with push obstacle mode initialized");
}

void loop() {
  // Read distance once
  float distance = getDistance();
  bool pushMode = (distance > 0 && distance < OBSTACLE_THRESHOLD_CM);

  // Read line sensor values
  bool leftOn  = digitalRead(LEFT_SENSOR)  == LOW;
  bool rightOn = digitalRead(RIGHT_SENSOR) == LOW;

  // Follow line with appropriate speeds
  followLine(leftOn, rightOn, pushMode);

  delay(10);
}

// Line follow logic with push mode
void followLine(bool leftOnLine, bool rightOnLine, bool pushMode) {
  int forwardSpeed = pushMode ? PUSH_BASE_SPEED : BASE_SPEED;
  int turnSpeed    = pushMode ? PUSH_TURN_SPEED : TURN_SPEED;
  
  // Both sensors on line: move straight
  if (leftOnLine && rightOnLine) {
    moveForward(forwardSpeed);
  }
  // Left sensor off but right on: turn left
  else if (!leftOnLine && rightOnLine) {
    turnLeft(turnSpeed);
  }
  // Right sensor off but left on: turn right
  else if (leftOnLine && !rightOnLine) {
    turnRight(turnSpeed);
  }
  // Both sensors off
  else {
    if (pushMode) {
      // Continue pushing forward
      moveForward(forwardSpeed);
    } else {
      // Lost line: back up to recover
      moveBackward(BACKWARD_SPEED);
    }
  }
}

// Motor control
void moveForward(int speed) {
  analogWrite(ENA, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENB, speed);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward(int speed) {
  analogWrite(ENA, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENB, speed);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft(int speed) {
  analogWrite(ENA, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENB, speed);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight(int speed) {
  analogWrite(ENA, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENB, speed);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Ultrasonic distance measurement (cm)
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 20000);
  if (duration == 0) return -1;  // no echo
  return (duration * 0.034) / 2;
}
