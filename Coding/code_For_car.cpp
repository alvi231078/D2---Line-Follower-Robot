// Motor control pins
const int motorPin1 = 8;
const int motorPin2 = 9;
const int enablePin = 10;

// Ultrasonic sensor
const int trigPin = 6;
const int echoPin = 7;

// Warning indicators
const int buzzerPin = 4;
const int ledPin = 5;

// IR sensor pins
const int irLeftPin = 2;
const int irRightPin = 3;

// Motor speeds
const int forwardSpeed = 180;
const int reverseSpeed = 160;

// Obstacle threshold (cm)
const int distanceThreshold = 15;

void setup() {
  setupMotorPins();
  setupSensorPins();
  setupWarningPins();
  setupIRPins();

  Serial.begin(9600);
}

void loop() {
  int distance = getDistance();                    // ultrasonic
  bool irLeft = digitalRead(irLeftPin) == LOW;     // obstacle on left?
  bool irRight = digitalRead(irRightPin) == LOW;   // obstacle on right?

  showDistance(distance);
  showIRSensors(irLeft, irRight);

  controlNavigation(distance, irLeft, irRight);     // new full control
  delay(200); // reduced for quicker response
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

void setupIRPins() {
  pinMode(irLeftPin, INPUT);
  pinMode(irRightPin, INPUT);
}

// Read distance from ultrasonic
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

// Serial debug: distance
void showDistance(int distance) {
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

// Serial debug: IR sensors
void showIRSensors(bool leftDetected, bool rightDetected) {
  Serial.print("IR Left: ");
  Serial.print(leftDetected ? "DETECTED" : "CLEAR");
  Serial.print(" | IR Right: ");
  Serial.println(rightDetected ? "DETECTED" : "CLEAR");
}

// Main control logic with IR + ultrasonic
void controlNavigation(int distance, bool irLeft, bool irRight) {
  if (distance <= distanceThreshold) {
    stopMotor();
    startWarning();  // object ahead!
    return;
  }

  stopWarning(); // ultrasonic clear

  if (irLeft && irRight) {
    stopMotor();  // both sides blocked
  } else if (irLeft) {
    turnRight();  // only left sensor blocked
  } else if (irRight) {
    turnLeft();   // only right sensor blocked
  } else {
    moveForward();  // nothing detected
  }
}

// Motor behaviors
void moveForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, forwardSpeed);
}

void stopMotor() {
  analogWrite(enablePin, 0);
}

void turnLeft() {
  // reverse one side to turn left (quick and dirty)
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  analogWrite(enablePin, reverseSpeed);
}

void turnRight() {
  // standard forward turn logic (same as left but flipped)
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, reverseSpeed);
}

// Warnings
void startWarning() {
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(ledPin, HIGH);
}

void stopWarning() {
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);
}
