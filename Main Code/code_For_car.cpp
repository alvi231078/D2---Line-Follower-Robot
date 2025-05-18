// Motor driver pins
const int ENA = 10;  // PWM Motor A (left)
const int ENB = 11;  // PWM Motor B (right)
const int IN1 = 5;   // Motor A input 1
const int IN2 = 6;   // Motor A input 2
const int IN3 = 7;   // Motor B input 1
const int IN4 = 8;   // Motor B input 2

// IR sensor pins
const int LEFT_SENSOR  = 3;
const int RIGHT_SENSOR = 4;

// Speed settings
const int BASE_SPEED    = 65;
const int TURN_SPEED    = 90;
const int REVERSE_SPEED = 60;

// Directions for readability
enum Dir { STOP = 0, FORWARD = 1, BACKWARD = -1 };

void setup() {
  // Motor pins
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Sensors
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);
}

// Utility to set each motor’s speed & direction.
// leftSpeed/rightSpeed = PWM 0–255
// leftDir/rightDir = FORWARD, BACKWARD, or STOP
void drive(int leftSpeed, Dir leftDir, int rightSpeed, Dir rightDir) {
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
  
  digitalWrite(IN1, leftDir == FORWARD);
  digitalWrite(IN2, leftDir == BACKWARD);
  digitalWrite(IN3, rightDir == FORWARD);
  digitalWrite(IN4, rightDir == BACKWARD);
}

void loop() {
  bool onLineL = digitalRead(LEFT_SENSOR)  == LOW;
  bool onLineR = digitalRead(RIGHT_SENSOR) == LOW;

  if (onLineL && onLineR) {
    // both sensors see line → go straight
    drive(BASE_SPEED, FORWARD, BASE_SPEED, FORWARD);
  }
  else if (!onLineL && onLineR) {
    // left sensor off line → spin left in place
    drive(TURN_SPEED, BACKWARD, TURN_SPEED, FORWARD);
  }
  else if (onLineL && !onLineR) {
    // right sensor off line → spin right in place
    drive(TURN_SPEED, FORWARD, TURN_SPEED, BACKWARD);
  }
  else {
    // lost the line entirely → back up
    drive(REVERSE_SPEED, BACKWARD, REVERSE_SPEED, BACKWARD);
  }
}
