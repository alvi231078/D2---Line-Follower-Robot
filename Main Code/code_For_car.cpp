#include <Servo.h>

// Motor driver pins
const int ENA = 10;  // PWM Motor A (left)
const int ENB = 11;  // PWM Motor B (right)
const int IN1 = 5;   // Motor A input 1
const int IN2 = 6;   // Motor A input 2
const int IN3 = 7;   // Motor B input 1
const int IN4 = 8;   // Motor B input 2

// IR sensor pins (digital)
const int LEFT_SENSOR = 3;
const int RIGHT_SENSOR = 4;

// Ultrasonic sensor pins
const int TRIG_PIN = 12;
const int ECHO_PIN = 13;

// Servo for scanning
const int SERVO_PIN = 9;
const int SERVO_LEFT = 145;
const int SERVO_CENTER = 67;
const int SERVO_RIGHT = 0;

// Speed settings (PWM 0-255) - Lowered for better stability
const int BASE_SPEED = 70;     // Normal forward speed
const int TURN_SPEED = 80;     // Speed during turns
const int BACKWARD_SPEED = 60; // Speed when moving backward

// Distance thresholds
const int OBSTACLE_THRESHOLD_CM = 6;

// Timing constants for maneuvers (in milliseconds)
const unsigned long REVERSE_DURATION = 1000;    // Time to back up
const unsigned long TURN_DURATION = 300;        // Time for each 45° turn
const unsigned long FORWARD_DURATION = 600;     // Time to move forward after turns
const unsigned long SERVO_MOVE_DELAY = 350;     // Time to wait for servo to move
const unsigned long SCAN_COMPLETE_DELAY = 200;  // Time after scan completes

// Robot states
enum RobotState {
  FOLLOW_LINE,           // Normal line following
  DETECT_OBSTACLE,       // Detected obstacle, preparing to handle
  SCAN_FOR_PATH,         // Scanning with servo to find path
  BACKUP,                // Moving backward
  TURN_AWAY,             // First 45° turn away from line
  MOVE_PAST,             // Moving forward past obstacle
  TURN_BACK,             // Second 45° turn back toward line
  FIND_LINE              // Moving forward to find line again
};

// Direction for obstacle avoidance
enum Direction {
  NONE,
  LEFT,
  RIGHT
};

// Global variables
RobotState currentState = FOLLOW_LINE;
Direction bypassDirection = NONE;
unsigned long stateStartTime = 0;
unsigned long currentTime = 0;
Servo scanner;

void setup() {
  // Set motor pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Set sensor pins
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Initialize servo
  scanner.attach(SERVO_PIN);
  scanner.write(SERVO_CENTER);
  
  // Initialize serial for debugging
  Serial.begin(9600);
  Serial.println("Line follower with obstacle avoidance initialized");
  
  // Initial delay to position robot
  delay(1000);
}

void loop() {
  // Get current time
  currentTime = millis();
  
  // Read line sensor values
  bool leftOnLine = isOnLine(LEFT_SENSOR);
  bool rightOnLine = isOnLine(RIGHT_SENSOR);
  
  // State machine for robot behavior
  switch (currentState) {
    case FOLLOW_LINE:
      // Check for obstacles
      if (getDistance() < OBSTACLE_THRESHOLD_CM) {
        // Obstacle detected, transition to obstacle detection
        transitionToState(DETECT_OBSTACLE);
        break;
      }
      
      // Normal line following
      followLine(leftOnLine, rightOnLine);
      break;
      
    case DETECT_OBSTACLE:
      // Stop motors and prepare to scan
      stopMotors();
      transitionToState(SCAN_FOR_PATH);
      break;
      
    case SCAN_FOR_PATH:
      // Scan for the best path around obstacle
      bypassDirection = scanForClearPath();
      
      // If we found a clear path, start the bypass sequence
      if (bypassDirection != NONE) {
        transitionToState(BACKUP);
      } else {
        // No clear path, default to right
        bypassDirection = RIGHT;
        transitionToState(BACKUP);
      }
      break;
      
    case BACKUP:
      // Back up straight for a set duration
      moveBackward(BACKWARD_SPEED);
      
      if (currentTime - stateStartTime >= REVERSE_DURATION) {
        stopMotors();
        transitionToState(TURN_AWAY);
      }
      break;
      
    case TURN_AWAY:
      // First 45° turn (away from line)
      if (bypassDirection == RIGHT) {
        turnRight(TURN_SPEED);
      } else {
        turnLeft(TURN_SPEED);
      }
      
      if (currentTime - stateStartTime >= TURN_DURATION) {
        stopMotors();
        transitionToState(MOVE_PAST);
      }
      break;
      
    case MOVE_PAST:
      // Move forward past the obstacle
      moveForward(BASE_SPEED);
      
      if (currentTime - stateStartTime >= FORWARD_DURATION) {
        stopMotors();
        transitionToState(TURN_BACK);
      }
      break;
      
    case TURN_BACK:
      // Second 45° turn (back toward line)
      if (bypassDirection == RIGHT) {
        turnLeft(TURN_SPEED);
      } else {
        turnRight(TURN_SPEED);
      }
      
      if (currentTime - stateStartTime >= TURN_DURATION) {
        stopMotors();
        transitionToState(FIND_LINE);
      }
      break;
      
    case FIND_LINE:
      // Move forward until we find the line again
      moveForward(BASE_SPEED);
      
      // Check if we found the line
      if (leftOnLine || rightOnLine) {
        // Back on the line, resume normal operation
        transitionToState(FOLLOW_LINE);
      }
      
      // Add timeout safety in case we miss the line
      if (currentTime - stateStartTime >= 2000) {
        // If we've been searching too long, try a different strategy
        // For now, just go back to following (could be more sophisticated)
        transitionToState(FOLLOW_LINE);
      }
      break;
  }
  
  // Small delay to stabilize readings
  delay(10);
}

// SENSOR FUNCTIONS

// Returns true if sensor detects line (LOW signal)
bool isOnLine(int sensorPin) {
  return digitalRead(sensorPin) == LOW;
}

// Get distance to obstacle in cm
int getDistance() {
  // Clear the trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Send 10μs pulse to trigger
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Measure response
  long duration = pulseIn(ECHO_PIN, HIGH, 20000);
  
  // Calculate distance in cm (speed of sound = 0.034 cm/μs)
  int distance = (duration == 0) ? 999 : (duration * 0.034 / 2);
  
  return distance;
}

// Scan with servo to find best path
Direction scanForClearPath() {
  int leftDistance, rightDistance;
  
  // Look left
  scanner.write(SERVO_LEFT);
  delay(SERVO_MOVE_DELAY);
  leftDistance = getDistance();
  
  // Look right
  scanner.write(SERVO_RIGHT);
  delay(SERVO_MOVE_DELAY);
  rightDistance = getDistance();
  
  // Return to center
  scanner.write(SERVO_CENTER);
  delay(SCAN_COMPLETE_DELAY);
  
  Serial.print("Scan - Left: ");
  Serial.print(leftDistance);
  Serial.print("cm, Right: ");
  Serial.print(rightDistance);
  Serial.println("cm");
  
  // Decide which way to go based on distance readings
  if (rightDistance > OBSTACLE_THRESHOLD_CM && rightDistance >= leftDistance) {
    return RIGHT;
  } else if (leftDistance > OBSTACLE_THRESHOLD_CM) {
    return LEFT;
  } else {
    // Both directions have obstacles, default to whichever has more space
    return (rightDistance >= leftDistance) ? RIGHT : LEFT;
  }
}

// LINE FOLLOWING FUNCTION

void followLine(bool leftOnLine, bool rightOnLine) {
  // Both sensors on the line - go straight
  if (leftOnLine && rightOnLine) {
    moveForward(BASE_SPEED);
  }
  // Left sensor off the line - turn left
  else if (!leftOnLine && rightOnLine) {
    turnLeft(TURN_SPEED);
  }
  // Right sensor off the line - turn right
  else if (leftOnLine && !rightOnLine) {
    turnRight(TURN_SPEED);
  }
  // Both sensors off the line - back up to find line
  else {
    moveBackward(BACKWARD_SPEED);
  }
}

// STATE MANAGEMENT

void transitionToState(RobotState newState) {
  currentState = newState;
  stateStartTime = currentTime;
  
  // Debug output
  Serial.print("State changed to: ");
  Serial.println(getStateName(newState));
}

String getStateName(RobotState state) {
  switch(state) {
    case FOLLOW_LINE: return "FOLLOW_LINE";
    case DETECT_OBSTACLE: return "DETECT_OBSTACLE";
    case SCAN_FOR_PATH: return "SCAN_FOR_PATH";
    case BACKUP: return "BACKUP";
    case TURN_AWAY: return "TURN_AWAY";
    case MOVE_PAST: return "MOVE_PAST";
    case TURN_BACK: return "TURN_BACK";
    case FIND_LINE: return "FIND_LINE";
    default: return "UNKNOWN";
  }
}

// MOTOR CONTROL FUNCTIONS

void moveForward(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH); // Left wheel backward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);  // Right wheel forward
}

void turnRight(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);  // Left wheel forward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); // Right wheel backward
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
