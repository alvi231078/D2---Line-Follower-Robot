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

  // Check if distance is around 100 cm
  if (distance >= 98 && distance <= 102) {
    Serial.println(">>> Distance is around 100 cm <<<");
  }

  // Obstacle too close? Stop!
  if (distance < 50.8) {
    Serial.println("⚠️ Obstacle ahead! Stopping...");
    stopMotors();
    return;
  }

  // Read IR sensors
  bool leftIR = digitalRead(IR_LEFT);   // HIGH = white, LOW = black
  bool rightIR = digitalRead(IR_RIGHT);

  Serial.print("IR Left: ");
  Serial.print(leftIR ? "WHITE" : "BLACK");
  Serial.print(" | IR Right: ");
  Serial.println(rightIR ? "WHITE" : "BLACK");

  // IR-based navigation
  if (!leftIR && !rightIR) {
    Serial.println("✅ On line: go straight");
    moveForward();  // both on black – follow line
  }
  else if (!leftIR && rightIR) {
    Serial.println("↪️ Adjusting left...");
    turnLeft();
  }
  else if (leftIR && !rightIR) {
    Serial.println("↩️ Adjusting right...");
    turnRight();
  }
  else {
    // Both sensors see white = lost the line
    Serial.println("🚫 Line lost! Trying to recover...");
    stopMotors();
    delay(100);
    recoverLine(); // new function
  }

  delay(100); // loop delay for stability
}

// Reads ultrasonic distance in cm
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

// Recovery spin: slow spin until any IR sees black
void recoverLine() {
  // Slow spin right
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);

  while (digitalRead(IR_LEFT) == HIGH && digitalRead(IR_RIGHT) == HIGH) {
    Serial.print(".");  // visual feedback
    delay(20);
  }

  stopMotors();
  Serial.println("\n✅ Line re-acquired!");
}
