// Motor control pins
const int motorPin1 = 8;
const int motorPin2 = 9;
const int enablePin = 10;

// Ultrasonic sensor pins (HC-SR04)
const int trigPin = 6;
const int echoPin = 7;

// Speed settings for the motor (0–255 range)
const int forwardSpeed = 180;
const int reverseSpeed = 160;

// Minimum safe distance from an obstacle (in centimeters)
const int distanceThreshold = 15;

void setup() {
  // Set motor pins as outputs
  setupMotorPins();

  // Set ultrasonic pins (trig = output, echo = input)
  setupUltrasonicPins();

  // Start serial monitor to see distance values
  Serial.begin(9600);
}

void loop() {
  // Measure how far the object is from the sensor
  int distance = getDistance();

  // Show the distance in the serial monitor
  printDistance(distance);

  // Decide what the motor should do based on the distance
  controlMotor(distance);

  // Wait a bit before repeating the loop
  delay(500);
}

// This sets the motor control pins as outputs
void setupMotorPins() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
}

// This sets up the ultrasonic sensor pins
void setupUltrasonicPins() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

// This function sends a pulse from the sensor and measures how long it takes to come back
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  // Measure the time it takes for the echo to return
  long duration = pulseIn(echoPin, HIGH);

  // Convert time (microseconds) to distance in cm
  int distance = duration * 0.034 / 2;
  return distance;
}

// Prints the measured distance in the serial monitor
void printDistance(int distance) {
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

// Turns the motor on or off depending on how close the object is
void controlMotor(int distance) {
  if (distance > distanceThreshold) {
    // If it's safe, move forward
    moveForward();
  } else {
    // Otherwise, stop the motor
    stopMotor();
  }
}

// Makes the motor spin forward
void moveForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, forwardSpeed);
}

// Stops the motor
void stopMotor() {
  analogWrite(enablePin, 0);
}
