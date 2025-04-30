// Motor control pins (L298N)
const int motorPin1 = 8;
const int motorPin2 = 9;
const int enablePin = 10;

// Ultrasonic sensor pins
const int trigPin = 6;
const int echoPin = 7;

// Buzzer and warning LED
const int buzzerPin = 4;
const int ledPin = 5;

// IR sensors (new)
const int irLeftPin = 2;
const int irRightPin = 3;

// Motor speeds
const int forwardSpeed = 180;
const int reverseSpeed = 160; // unused still

// Obstacle threshold for ultrasonic (cm)
const int distanceThreshold = 15;

void setup() {
  setupMotorPins();
  setupSensorPins();
  setupWarningPins();
  setupIRPins(); // new IR setup

  Serial.begin(9600); // view data in Serial Monitor
}

void loop() {
  int distance = getDistance();     // check ultrasonic
  bool irLeft = digitalRead(irLeftPin) == LOW;   // detect on left
  bool irRight = digitalRead(irRightPin) == LOW; // detect on right

  showDistance(distance);
  showIRSensors(irLeft, irRight);  // new function

  controlMotor(distance);          // still reacts to ultrasonic

  delay(500);
}

// Setup functions
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

// Set IR sensor pins
void setupIRPins() {
  pinMode(irLeftPin, INPUT);
  pinMode(irRightPin, INPUT);
}

// Read distance with ultrasonic sensor
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}

// Show distance on serial monitor
void showDistance(int distance) {
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

// Show IR sensor readings
void showIRSensors(bool leftDetected, bool rightDetected) {
  Serial.print("IR Left: ");
  Serial.print(leftDetected ? "DETECTED" : "CLEAR");
  Serial.print(" | IR Right: ");
  Serial.println(rightDetected ? "DETECTED" : "CLEAR");
}

// Motor logic based on ultrasonic (IR not affecting movement yet)
void controlMotor(int distance) {
  if (distance > distanceThreshold) {
    moveForward();
    stopWarning();
  } else {
    stopMotor();
    startWarning();
  }
}

// Motor control
void moveForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, forwardSpeed);
}

void stopMotor() {
  analogWrite(enablePin, 0);
}

// Alerts
void startWarning() {
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(ledPin, HIGH);
}

void stopWarning() {
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);
}
