// Motor driver pins
const int ENA = 10;  // PWM Motor A (left)
const int ENB = 11;  // PWM Motor B (right)
const int IN1 = 5;   // Motor A input 1
const int IN2 = 6;   // Motor A input 2
const int IN3 = 7;   // Motor B input 1
const int IN4 = 8;   // Motor B input 2

// IR sensor pins (digital)
const int leftSensorPin = 3;
const int rightSensorPin = 4;

// Speed settings (PWM 0-255)
const int baseSpeed = 65;
const int maxSpeed = 90;
const int reverseSpeed = 60;

void setup() {
  // Set motor pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Set sensor pins as inputs
  pinMode(leftSensorPin, INPUT);
  pinMode(rightSensorPin, INPUT);
}

void loop() {
  // Read sensor values
  int leftSensor = digitalRead(leftSensorPin);
  int rightSensor = digitalRead(rightSensorPin);

  // If both sensors are on the line, move forward
  if (leftSensor == LOW && rightSensor == LOW) {
    moveForward(baseSpeed);
  }
  // If only the left sensor is off the line, turn left
  else if (leftSensor == HIGH && rightSensor == LOW) {
    turnLeft(baseSpeed);
  }
  // If only the right sensor is off the line, turn right
  else if (leftSensor == LOW && rightSensor == HIGH) {
    turnRight(baseSpeed);
  }
  // If both sensors are off the line, reverse and search for the line
  else {
    moveBackward(reverseSpeed);
  }
}

// Function to move forward
void moveForward(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Function to turn left
void turnLeft(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Function to turn right
void turnRight(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// Function to move backward
void moveBackward(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
