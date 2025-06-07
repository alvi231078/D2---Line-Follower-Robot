// Define motor control pins
#define enA 5   // Enable pin for Motor A
#define in1 6   // Motor A input 1
#define in2 7   // Motor A input 2
#define in3 9   // Motor B input 1
#define in4 10  // Motor B input 2
#define enB 8   // Enable pin for Motor B

// Define sensor input pins
#define R_S 4   // Right sensor pin
#define L_S 2   // Left sensor pin

void setup() {
  // Set motor control pins as outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);

  // Set sensor pins as inputs
  pinMode(R_S, INPUT);
  pinMode(L_S, INPUT);

  // Turn on both motors by setting enable pins high
  digitalWrite(enA, HIGH);
  digitalWrite(enB, HIGH);
}

void loop() {
  // Read values from left and right sensors
  int right = digitalRead(R_S);
  int left = digitalRead(L_S);

  // Determine movement direction based on sensor input
  if (right == 0 && left == 0) {
    // Both sensors detect the line: go straight
    forward();
  } 
  else if (right == 1 && left == 0) {
    // Right sensor off the line, left on: turn right
    turnRight();
  } 
  else if (right == 0 && left == 1) {
    // Left sensor off the line, right on: turn left
    turnLeft();
  } 
  else {
    // Both sensors off the line: stop
    stopMotors();
  }
}

void forward() {
  // Rotate both motors forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnRight() {
  // Rotate left motor forward, stop right motor
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnLeft() {
  // Rotate right motor forward, stop left motor
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void stopMotors() {
  // Stop both motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
