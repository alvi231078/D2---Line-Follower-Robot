// Motor control pins
const int motorPin1 = 8;
const int motorPin2 = 9;
const int enablePin = 10;

// Ultrasonic sensor
const int trigPin = 6;
const int echoPin = 7;

// Alerts
const int buzzerPin = 4;
const int ledPin = 5;

// IR sensors (line follower)
const int irLeftPin = 2;
const int irRightPin = 3;

// Speeds
const int forwardSpeed = 180;
const int turnSpeed = 140;
const int reverseSpeed = 120;

// Obstacle distance threshold
const int distanceThreshold = 15;

void setup() {
  setupMotorPins();
  setupSensorPins();
  setupWarningPins();
  setupIRPins();
  Serial.begin(9600);
}

void loop() {
  int distance = getDistance();
  bool irLeft = digitalRead(irLeftPin) == LOW;
  bool irRight = digitalRead(irRightPin) == LOW;

  showDistance(distance);
  showIRSensors(irLeft, irRight);

  controlNavigation(distance, irLeft, irRight);
  delay(100);
}

void setupMotorPins() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
}

void setupSensorPins() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void setupWarningPins() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void setupIRPins() {
  pinMode(irLeftPin, INPUT);
  pinMode(irRightPin, INPUT);
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

void showDistance(int distance) {
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

void showIRSensors(bool leftDetected, bool rightDetected) {
  Serial.print("IR Left: ");
  Serial.print(leftDetected ? "LINE" : "WHITE");
  Serial.print(" | IR Right: ");
  Serial.println(rightDetected ? "LINE" : "WHITE");
}

void controlNavigation(int distance, bool irLeft, bool irRight) {
  if (distance <= distanceThreshold) {
    stopMotor();
    startWarning();
    return;
  }

  stopWarning(); // no object ahead

  if (irLeft && irRight) {
    moveForward(); // both on line
  } else if (irLeft && !irRight) {
    turnLeft(); // adjust left
  } else if (!irLeft && irRight) {
    turnRight(); // adjust right
  } else {
    // both see white – robot is off the line
    stopMotor();
    delay(150);        // pause to think
    moveBackward();    // back off a little
    delay(300);
    spinRight();       // try to recover by spinning
    delay(400);
    stopMotor();       // pause again
  }
}

// Drive logic
void moveForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, forwardSpeed);
}

void turnLeft() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  analogWrite(enablePin, turnSpeed);
}

void turnRight() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, turnSpeed);
}

void moveBackward() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  analogWrite(enablePin, reverseSpeed);
}

void spinRight() {
  // pulse spin to re-find the line
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, HIGH); // both high = spin on some motor drivers
  analogWrite(enablePin, turnSpeed);
}

void stopMotor() {
  analogWrite(enablePin, 0);
}

void startWarning() {
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(ledPin, HIGH);
}

void stopWarning() {
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);
}
