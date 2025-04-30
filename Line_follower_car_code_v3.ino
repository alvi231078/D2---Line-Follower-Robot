// Motor Pin Definitions
#define motorPin1 9    // Motor 1 control pin (IN1 on L293D)
#define motorPin2 10   // Motor 2 control pin (IN2 on L293D)
#define motorPin3 5    // Motor 3 control pin (IN3 on L293D)
#define motorPin4 6    // Motor 4 control pin (IN4 on L293D)

// Ultrasonic Pin Definitions
#define TRIG_PIN 7   // Trigger pin for Ultrasonic sensor
#define ECHO_PIN 8   // Echo pin for Ultrasonic sensor

// IR Sensor Pin Definitions
#define IR_LEFT 2     // Left IR sensor output
#define IR_RIGHT 3    // Right IR sensor output

void setup() {
  // Initialize Motor Control Pins
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  // Initialize Ultrasonic Sensor Pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Initialize IR Sensor Pins
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  Serial.begin(9600); // Debugging
}

void loop() {
  long distance = getDistance();

  // If an obstacle is close, stop
  if (distance < 50.8) {
    stopMotors();
    return;
  }

  // Read IR Sensors
  bool leftIR = digitalRead(IR_LEFT);   // HIGH = white, LOW = black
  bool rightIR = digitalRead(IR_RIGHT);

  Serial.print("Left IR: ");
  Serial.print(leftIR);
  Serial.print(" | Right IR: ");
  Serial.println(rightIR);

  // Line following logic
  if (leftIR == LOW && rightIR == LOW) {
    stopMotors(); // Could be intersection or dead end
  }
  else if (leftIR == LOW && rightIR == HIGH) {
    turnLeft();
  }
  else if (rightIR == LOW && leftIR == HIGH) {
    turnRight();
  }
  else {
    moveForward(); // both sensors on white
  }
}

// Function to get distance from Ultrasonic Sensor
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2; // in cm
}

// Movement Functions
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
  digitalWrite(motorPin2, HIGH);  // Reverse left motor
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);   // Right motor forward
}

void turnRight() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);   // Left motor forward
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);  // Reverse right motor
}
