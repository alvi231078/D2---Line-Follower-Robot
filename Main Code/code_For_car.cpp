// Motor driver pins
const int IN1 = 5;  // Motor A input 1
const int IN2 = 6;  // Motor A input 2
const int IN3 = 7;  // Motor B input 1
const int IN4 = 8;  // Motor B input 2

// Note: ENA and ENB are tied to 5V directly, so no PWM control here

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  // Move motors forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(3000);

  // Move mot// Motor driver pins
const int ENA = 10;  // PWM pin for Motor A speed
const int ENB = 11;  // PWM pin for Motor B speed
const int IN1 = 5;   // Motor A input 1
const int IN2 = 6;   // Motor A input 2
const int IN3 = 7;   // Motor B input 1
const int IN4 = 8;   // Motor B input 2

// IR sensor pins
const int leftSensor = 3;   // Digital IR sensor left
const int rightSensor = 4;  // Digital IR sensor right

void setup() {
  // Motor pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // IR sensors as inputs
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);

  Serial.begin(9600);
}

void loop() {
  int leftState = digitalRead(leftSensor);
  int rightState = digitalRead(rightSensor);

  Serial.print("Left IR: ");
  Serial.print(leftState);
  Serial.print(" | Right IR: ");
  Serial.println(rightState);

  if (leftState == 1 && rightState == 1) {
    // Both sensors see white → move forward
    moveForward();
  } 
  else if (leftState == 0 && rightState == 1) {
    // Left sensor sees black → turn left
    turnLeft();
  } 
  else if (leftState == 1 && rightState == 0) {
    // Right sensor sees black → turn right
    turnRight();
  } 
  else {
    // Both sensors see black → stop
    stopMotors();
  }
}

// Move forward function
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 255);  // Full speed
  analogWrite(ENB, 255);
}

// Turn left function
void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 255);
}

// Turn right function
void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 255);
  analogWrite(ENB, 0);
}

// Stop function
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
ors backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(3000);

  // Stop motors
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(2000);
}
