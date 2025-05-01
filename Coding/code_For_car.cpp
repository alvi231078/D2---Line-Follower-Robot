// Motor pins
#define motorPin1 9
#define motorPin2 10
#define motorPin3 5
#define motorPin4 6

// Ultrasonic sensor pins
#define TRIG_PIN 7
#define ECHO_PIN 8

// IR sensor pins
#define IR_LEFT 2
#define IR_RIGHT 3

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);
}

void loop() {
  long distance = getDistance();
  if (distance < 30) {
    stopMotors(); // obstacle too close
    return;
  }

  bool leftIR = digitalRead(IR_LEFT);   // HIGH = white
  bool rightIR = digitalRead(IR_RIGHT);

  if (!leftIR && !rightIR) {
    moveForward(); // both on line
  }
  else if (!leftIR && rightIR) {
    turnLeft(); // right drift, correct left
  }
  else if (leftIR && !rightIR) {
    turnRight(); // left drift, correct right
  }
  else {
    // Both sensors see white: robot lost the line
    recoverLine();
  }

  delay(10); // smooth loop
}

// Distance in cm using ultrasonic
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

// Movement functions
void moveForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void stopMotors() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}

void turnLeft() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void turnRight() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
}

// Improved recovery routine when line is lost
void recoverLine() {
  stopMotors();
  delay(100);

  // Step 1: reverse slightly
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  delay(250);

  // Step 2: spin slowly until line is found again
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);

  while (digitalRead(IR_LEFT) == HIGH && digitalRead(IR_RIGHT) == HIGH) {
    delay(10);
  }

  stopMotors();
}
