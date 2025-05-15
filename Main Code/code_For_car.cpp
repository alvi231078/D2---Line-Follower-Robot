// Motor Driver Pins
const int ENA = 10;  // If tied to 5V, replace with actual PWM pin or set HIGH
const int ENB = 11;  // Same as above
const int IN1 = 5;
const int IN2 = 6;
const int IN3 = 7;
const int IN4 = 8;

// IR Sensor Pins
const int leftSensor = 3;  // Digital IR sensor left
const int rightSensor = 4; // Digital IR sensor right

void setup() {
  // Initialize motor pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Initialize IR sensor pins as inputs
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);
  
  // Start with motors stopped
  stopMotors();
}

void loop() {
  int leftState = digitalRead(leftSensor);
  int rightState = digitalRead(rightSensor);

  // Logic based on IR sensors (0 = black line, 1 = white surface)
  if (leftState == 1 && rightState == 1) {
    // Both sensors on white surface → move forward
    moveForward();
  }
  else if (leftState == 0 && rightState == 1) {
    // Left sensor on black line → turn left
    turnLeft();
  }
  else if (leftState == 1 && rightState == 0) {
    // Right sensor on black line → turn right
    turnRight();
  }
  else {
    // Both sensors on black → stop (or you can implement a U-turn or search)
    stopMotors();
  }
}

// Motor control functions
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  
  analogWrite(ENA, 255);  // Full speed
  analogWrite(ENB, 255);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
