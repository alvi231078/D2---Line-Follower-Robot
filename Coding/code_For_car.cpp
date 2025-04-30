// motor pins again
const int motorPin1 = 8;
const int motorPin2 = 9;
const int enablePin = 10;

// ultrasonic sensor (HC-SR04)
const int trigPin = 6;
const int echoPin = 7;

// new stuff: buzzer + warning LED
const int buzzerPin = 4;
const int ledPin = 5;

// motor speed values – might tweak later
const int forwardSpeed = 180;
const int reverseSpeed = 160;  // unused still, oops

const int distanceThreshold = 15;  // if closer than this → stop + alert

void setup() {
  setupMotorPins();
  setupSensorPins();
  setupWarningPins();  // new: for buzzer + LED

  Serial.begin(9600);  // serial monitor to see distance
}

void loop() {
  int distance = getDistance();   // ping the sensor
  showDistance(distance);         // print result
  controlMotor(distance);         // act on result
  delay(500);  // maybe reduce for quicker reaction?
}

// motor pins setup
void setupMotorPins() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
}

// ultrasonic pins (trig out, echo in)
void setupSensorPins() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

// set up LED and buzzer pins as outputs
void setupWarningPins() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

// measure distance using ultrasonic
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);  // time for echo
  int distance = duration * 0.034 / 2;     // convert to cm
  return distance;
}

// print distance to serial
void showDistance(int distance) {
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

// if object far: go forward
// if close: stop + alert
void controlMotor(int distance) {
  if (distance > distanceThreshold) {
    moveForward();
    stopWarning();  // no need to panic
  } else {
    stopMotor();
    startWarning();  // beep + blink
  }
}

// motor go brrr
void moveForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, forwardSpeed);
}

// motor no go
void stopMotor() {
  analogWrite(enablePin, 0);
}

// buzzer and LED ON
void startWarning() {
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(ledPin, HIGH);
}

// buzzer and LED OFF
void stopWarning() {
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);
}
// motor pins again
const int motorPin1 = 8;
const int motorPin2 = 9;
const int enablePin = 10;

// ultrasonic sensor (HC-SR04)
const int trigPin = 6;
const int echoPin = 7;

// new stuff: buzzer + warning LED
const int buzzerPin = 4;
const int ledPin = 5;

// motor speed values – might tweak later
const int forwardSpeed = 180;
const int reverseSpeed = 160;  // unused still, oops

const int distanceThreshold = 15;  // if closer than this → stop + alert

void setup() {
  setupMotorPins();
  setupSensorPins();
  setupWarningPins();  // new: for buzzer + LED

  Serial.begin(9600);  // serial monitor to see distance
}

void loop() {
  int distance = getDistance();   // ping the sensor
  showDistance(distance);         // print result
  controlMotor(distance);         // act on result
  delay(500);  // maybe reduce for quicker reaction?
}

// motor pins setup
void setupMotorPins() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
}

// ultrasonic pins (trig out, echo in)
void setupSensorPins() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

// set up LED and buzzer pins as outputs
void setupWarningPins() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

// measure distance using ultrasonic
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);  // time for echo
  int distance = duration * 0.034 / 2;     // convert to cm
  return distance;
}

// print distance to serial
void showDistance(int distance) {
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

// if object far: go forward
// if close: stop + alert
void controlMotor(int distance) {
  if (distance > distanceThreshold) {
    moveForward();
    stopWarning();  // no need to panic
  } else {
    stopMotor();
    startWarning();  // beep + blink
  }
}

// motor go brrr
void moveForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, forwardSpeed);
}

// motor no go
void stopMotor() {
  analogWrite(enablePin, 0);
}

// buzzer and LED ON
void startWarning() {
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(ledPin, HIGH);
}

// buzzer and LED OFF
void stopWarning() {
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);
}
