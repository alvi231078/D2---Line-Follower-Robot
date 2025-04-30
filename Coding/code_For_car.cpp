// Motor Pins
const int motorPin1 = 8;
const int motorPin2 = 9;
const int enablePin = 10;

// Ultrasonic Sensor Pins
const int trigPin = 6;
const int echoPin = 7;

// Dc mMotor speed
const int forwardSpeed = 180;
const int reverseSpeed = 160;

void setup() {
  // Motor pin setup
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);

  // Ultrasonic pin setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  long duration;
  int distance;

  // Triggers ultrasonic sensorr
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echooo time and convert to distance
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // cm

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 15) {
    // Forward
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    analogWrite(enablePin, forwardSpeed);
  } else {
    // Stop motor if obstacle detected
    analogWrite(enablePin, 0);
  }

  delay(500); // small gives a dely befor the next loopp
}
