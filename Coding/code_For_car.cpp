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
  // Motor pins
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  // Ultrasonic
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // IR sensors
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);
}

void loop() {
  // Distance sensor (optional, can disable if not needed)
  long distance = getDistance();
  if (distance < 30) {
    stopMotors(); // obstacle too close
    return;
  }

  // Read IR sensors
  bool leftIR = digitalRead(IR_LEFT);   // HIGH = white, LOW = black
  bool rightIR = digitalRead(IR_RIGHT);

  // Core line following logic
  if (!leftIR && !rightIR) {
    // Both on black – go straight
    moveForward();
  }
  else if (!leftIR && rightIR) {
    // Left sees line – turn left
    turnLeft();
  }
  else if (leftIR && !rightIR) {
    // Right sees line – turn right
    turnRight();
  }
  else {
    // Both on white – line lost, recover
    recoverLine();
  }

  delay(10); // small delay for smoother behavior
}

// Distance reading with ultrasonic sensor
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

// Motor movement functions
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

// Try to find the line again by spinning right slowly
void recoverLine() {
  // Spin right slowly until either sensor sees line again
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);

  while (digitalRead(IR_LEFT) == HIGH && digitalRead(IR_RIGHT) == HIGH) {
    delay(10); // short pause
  }

  stopMotors(); // re-aligned with line
}
