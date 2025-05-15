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

  // Move motors backward
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
