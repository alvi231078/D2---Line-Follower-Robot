const int motorPin1 = 8;   
const int motorPin2 = 9;   
const int enablePin = 10;  

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
}

void loop() {
  // motor frward
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, 180);
  delay(2000);

  // stop
  analogWrite(enablePin, 0);
  delay(1000);

  // revers dirction
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  analogWrite(enablePin, 160);
  delay(2000);

  // stop again
  analogWrite(enablePin, 0);
  delay(1000);
}
// This cde is for controlling a DC motor using an Arduino. It uses PWM to control the speed and direction of the motor.