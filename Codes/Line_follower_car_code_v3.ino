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
  // Setup motor pins
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  // Setup ultrasonic pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Setup IR sensors
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  Serial.begin(9600); // Start serial monitor
}

void loop() {
  long distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Check if distance is approx. 100 cm (±2 cm)
  if (distance >= 98 && distance <= 102) {
    Serial.println(">>> Distance is around 100 cm <<<");
  }

  // Obstacle check
  if (distance < 50.8) {
    stopMotors();
    return;
  }

  // Read IR sensors
  bool leftIR = digitalRead(IR_LEFT);
  bool rightIR = digitalRead(IR_RIGHT);

  Serial.print("L: ");
  Serial.print(leftIR);
  Serial.print(" | R: ");
  Serial.println(rightIR);

  // Line following
  if (!leftIR && !rightIR) {
    stopMotors();  // Both on black
  }
  else if (!leftIR && rightIR) {
    turnLeft();
  }
  else if (!rightIR && leftIR) {
    turnRight();
  }
  else {
    moveForward(); // Both on white
  }

  delay(100); // Delay for stability
}

// Read distance in cm
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

// Forward
void moveForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

// Stop
void stopMotors() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}

// Left turn
void turnLeft() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

// Right turn
void turnRight() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
}
