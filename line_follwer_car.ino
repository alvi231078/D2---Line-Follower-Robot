// Pin definitions
#define enA 5    // Enable Pin for Motor 1
#define in1 6    // Motor 1 control Pin 1
#define in2 7    // Motor 1 control Pin 2
#define in3 9    // Motor 2 control Pin 1
#define in4 10   // Motor 2 control Pin 2
#define enB 8    // Enable Pin for Motor 2
#define R_S 4    // Right IR sensor
#define L_S 2    // Left IR sensor

void setup() {
  // Set motor control pins as OUTPUT
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  
  // Set IR sensor pins as INPUT
  pinMode(R_S, INPUT);
  pinMode(L_S, INPUT);
  
  // Activate motor drivers
  digitalWrite(enA, HIGH);
  digitalWrite(enB, HIGH);
}

void loop() {
  // Read IR sensors
  int rightSensor = digitalRead(R_S);
  int leftSensor = digitalRead(L_S);
  
  // Determine motor actions based on sensor input
  if (rightSensor == 0 && leftSensor == 0) {
    forward();
  } 
  else if (rightSensor == 1 && leftSensor == 0) {
    turnRight();
  } 
  else if (rightSensor == 0 && leftSensor == 1) {
    turnLeft();
  } 
  else if (rightSensor == 1 && leftSensor == 1) {
    stopMotors();
  }
}

void forward() {
  // Move motors forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnRight() {
  // Turn motors to the right
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnLeft() {
  // Turn motors to the left
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void stopMotors() {
  // Stop motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
