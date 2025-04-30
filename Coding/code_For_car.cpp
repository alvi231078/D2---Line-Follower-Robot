// Motor pins (connected to L298N)
const int motorPin1 = 8;  // IN1
const int motorPin2 = 9;  // IN2
const int enablePin = 10; // ENA (PWM)

// Ultrasonic sensor (HC-SR04)
const int trigPin = 6;    // sends pulse
const int echoPin = 7;    // listens for echo

// Extra warning system
const int buzzerPin = 4;  // annoying beep
const int ledPin = 5;     // red LED for drama

// Speeds for motor (PWM values: 0–255)
const int forwardSpeed = 180;
const int reverseSpeed = 160;  // not using it rn but maybe later

// How close is too close? (in cm)
const int distanceThreshold = 15;

void setup() {
  // setting up all pins we’ll be using
  setupMotorPins();
  setupSensorPins();
  setupWarningPins();

  // open serial port so we can see distances
  Serial.begin(9600);
}

void loop() {
  // step 1: measure distance
  int distance = getDistance();

  // step 2: show it in Serial Monitor
  showDistance(distance);

  // step 3: react based on how close the object is
  controlMotor(distance);

  // short pause – sensor doesn’t need to spam
  delay(500);
}

// tell Arduino which pins control the motor
void setupMotorPins() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
}

// sensor setup – trig OUT, echo IN
void setupSensorPins() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

// buzzer and LED also need pinMode
void setupWarningPins() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

// this is where we measure the distance
int getDistance() {
  // clean trigger pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // send pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // wait for echo and calculate how long it took
  long duration = pulseIn(echoPin, HIGH);

  // convert time to cm (0.034 cm/microsec is speed of sound)
  int distance = duration * 0.034 / 2;

  return distance;
}

// just shows distance in Serial Monitor
void showDistance(int distance) {
  Serial.print("Object is ");
  Serial.print(distance);
  Serial.println(" cm away");
}

// decide what to do with the motor based on distance
void controlMotor(int distance) {
  if (distance > distanceThreshold) {
    moveForward();     // it's safe, keep going
    stopWarning();     // nothing scary nearby
  } else {
    stopMotor();       // too close, stop!
    startWarning();    // time to beep and blink
  }
}

// move forward – IN1 high, IN2 low
void moveForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, forwardSpeed);
}

// cut the power (could also add reverse here later)
void stopMotor() {
  analogWrite(enablePin, 0);
}

// warning system ON – sound and light
void startWarning() {
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(ledPin, HIGH);
}

// warning system OFF – peace restored
void stopWarning() {
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);
}
