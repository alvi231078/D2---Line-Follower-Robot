
// Motor pins
#define enA 5
#define in1 6
#define in2 7
#define in3 9
#define in4 10
#define enB 8

// Sensor pins
#define R_S 4
#define L_S 2

void setup() {
  // Motor pins as output
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);

  // Sensor pins as input
  pinMode(R_S, INPUT);
  pinMode(L_S, INPUT);

  // Enable motors
  digitalWrite(enA, HIGH);
  digitalWrite(enB, HIGH);
}

void loop() {
  // Read sensors
  int right = digitalRead(R_S);
  int left = digitalRead(L_S);

  // Decide movement
  if (right == 0 && left == 0) {
    forward();
  } 
  else if (right == 1 && left == 0) {
    turnRight();
  } 
  else if (right == 0 && left == 1) {
    turnLeft();
  } 
  else {
    stopMotors();
  }
}

void forward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnRight() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnLeft() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void stopMotors() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
