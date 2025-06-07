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

// Servo and Ultrasonic pins
const int SERVO_PIN = 9;
const int TRIG_PIN = 12;
const int ECHO_PIN = 13;

// Servo positions
const int SERVO_LEFT = 145;
const int SERVO_CENTER = 67;
const int SERVO_RIGHT = 0;

// Speed settings
const int BASE_SPEED    = 80;   
const int TURN_SPEED    = 75;   
const int REVERSE_SPEED = 65;   
const int SEARCH_SPEED  = 60;   // Speed for line searching

// Distance settings
const int OBSTACLE_DISTANCE = 15;  // Stop if obstacle within 15cm
const int MIN_SAFE_DISTANCE = 25;  // Minimum safe distance for path selection

// Sensor interpretation (adjust based on your IR sensors)
const bool LINE_DETECTED_STATE = false;  // false = LOW means line detected

// Timing for responsiveness
const int LOOP_DELAY = 8;          // Very fast loop - 8ms
const int SENSOR_READ_INTERVAL = 2; // Read sensors every 2ms for better reliability

// Variables for non-blocking operation
unsigned long lastSensorRead = 0;
unsigned long lastDebugPrint = 0;
unsigned long lastDistanceCheck = 0;
const int DEBUG_INTERVAL = 200;    // Print debug info every 200ms
const int DISTANCE_CHECK_INTERVAL = 50; // Check distance every 50ms

// Sensor state variables with filtering
bool leftOnLine = false;
bool rightOnLine = false;
bool prevLeftOnLine = false;
bool prevRightOnLine = false;

// Sensor filtering to reduce false readings
const int SENSOR_SAMPLES = 3;      // Number of samples to average
int leftSensorHistory[SENSOR_SAMPLES] = {0};
int rightSensorHistory[SENSOR_SAMPLES] = {0};
int sampleIndex = 0;

// Sensor reliability tracking
unsigned long rightSensorLastDetected = 0;
unsigned long leftSensorLastDetected = 0;
bool rightSensorReliable = true;

// Obstacle avoidance variables
bool obstacleDetected = false;
bool isAvoidingObstacle = false;
bool isSearchingForLine = false;
int currentDistance = 0;

// Include Servo library
#include <Servo.h>
Servo neckServo;

// Robot states
enum RobotState {
  LINE_FOLLOWING,
  OBSTACLE_DETECTED,
  SCANNING,
  AVOIDING_OBSTACLE,
  SEARCHING_LINE
};

RobotState currentState = LINE_FOLLOWING;

void setup() {
  // Motor pins
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // IR sensors
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);
  
  // Ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Servo setup
  neckServo.attach(SERVO_PIN);
  neckServo.write(SERVO_CENTER);
  delay(500);

  // Serial monitor
  Serial.begin(9600);
  Serial.println("Line Following Robot with Obstacle Avoidance");
  Serial.println("Initializing...");
  
  stopMotors();
  delay(1000);
  
  Serial.println("Started - continuous sensor monitoring");
}

void loop() {
  unsigned long currentTime = millis();
  
  // Read sensors at high frequency for maximum responsiveness
  if (currentTime - lastSensorRead >= SENSOR_READ_INTERVAL) {
    readSensors();
    lastSensorRead = currentTime;
  }
  
  // Check distance periodically
  if (currentTime - lastDistanceCheck >= DISTANCE_CHECK_INTERVAL) {
    currentDistance = measureDistance();
    lastDistanceCheck = currentTime;
  }
  
  // State machine for robot behavior
  switch (currentState) {
    case LINE_FOLLOWING:
      if (currentDistance <= OBSTACLE_DISTANCE && currentDistance > 0) {
        currentState = OBSTACLE_DETECTED;
        Serial.println("STATE: Obstacle detected!");
      } else {
        executeLineFollowing();
      }
      break;
      
    case OBSTACLE_DETECTED:
      stopMotors();
      delay(200);
      currentState = SCANNING;
      break;
      
    case SCANNING:
      performObstacleScan();
      break;
      
    case AVOIDING_OBSTACLE:
      // Continue moving in chosen direction until obstacle is cleared
      if (currentDistance > MIN_SAFE_DISTANCE || currentDistance == 0) {
        currentState = SEARCHING_LINE;
        Serial.println("STATE: Obstacle cleared, searching for line");
      }
      break;
      
    case SEARCHING_LINE:
      searchForLine();
      break;
  }
  
  // Debug output at reasonable interval
  if (currentTime - lastDebugPrint >= DEBUG_INTERVAL) {
    printDebugInfo();
    lastDebugPrint = currentTime;
  }
  
  delay(LOOP_DELAY);
}

void readSensors() {
  // Store previous state
  prevLeftOnLine = leftOnLine;
  prevRightOnLine = rightOnLine;
  
  // Multiple sensor readings for reliability
  int leftTotal = 0, rightTotal = 0;
  
  // Take multiple quick readings
  for(int i = 0; i < 3; i++) {
    leftTotal += digitalRead(LEFT_SENSOR);
    rightTotal += digitalRead(RIGHT_SENSOR);
    delayMicroseconds(500); // 0.5ms between readings
  }
  
  // Use majority vote for more reliable reading
  bool leftRaw = (leftTotal >= 2);   // True if 2 or more readings were HIGH
  bool rightRaw = (rightTotal >= 2); // True if 2 or more readings were HIGH
  
  // Apply filtering using history
  leftSensorHistory[sampleIndex] = leftRaw;
  rightSensorHistory[sampleIndex] = rightRaw;
  
  // Calculate filtered values
  int leftSum = 0, rightSum = 0;
  for(int i = 0; i < SENSOR_SAMPLES; i++) {
    leftSum += leftSensorHistory[i];
    rightSum += rightSensorHistory[i];
  }
  
  // Use majority vote from history
  bool leftFiltered = (leftSum >= (SENSOR_SAMPLES / 2 + 1));
  bool rightFiltered = (rightSum >= (SENSOR_SAMPLES / 2 + 1));
  
  // Interpret sensor readings
  leftOnLine = (leftFiltered == LINE_DETECTED_STATE);
  rightOnLine = (rightFiltered == LINE_DETECTED_STATE);
  
  // Track sensor reliability
  if(rightOnLine) {
    rightSensorLastDetected = millis();
    rightSensorReliable = true;
  } else if(millis() - rightSensorLastDetected > 5000) {
    rightSensorReliable = false;
  }
  
  // Update sample index
  sampleIndex = (sampleIndex + 1) % SENSOR_SAMPLES;
}

int measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
  if (duration == 0) return 0; // No echo received
  
  int distance = duration * 0.034 / 2;
  return distance;
}

void performObstacleScan() {
  Serial.println("Scanning for best path...");
  
  // Scan left
  neckServo.write(SERVO_LEFT);
  delay(300);
  int leftDistance = measureDistance();
  
  // Scan right  
  neckServo.write(SERVO_RIGHT);
  delay(300);
  int rightDistance = measureDistance();
  
  // Return to center
  neckServo.write(SERVO_CENTER);
  delay(300);
  
  Serial.print("Left: ");
  Serial.print(leftDistance);
  Serial.print("cm, Right: ");
  Serial.print(rightDistance);
  Serial.println("cm");
  
  // Choose direction with more space
  if (leftDistance > rightDistance && leftDistance > MIN_SAFE_DISTANCE) {
    Serial.println("Turning left to avoid obstacle");
    avoidObstacleLeft();
  } else if (rightDistance > MIN_SAFE_DISTANCE) {
    Serial.println("Turning right to avoid obstacle");
    avoidObstacleRight();
  } else {
    Serial.println("No clear path, backing up");
    moveBackward();
    delay(800);
    rotateRight(TURN_SPEED);
    delay(500);
  }
  
  currentState = AVOIDING_OBSTACLE;
}

void avoidObstacleLeft() {
  // Turn left and move forward
  rotateLeft(TURN_SPEED);
  delay(600);
  moveForward(BASE_SPEED, BASE_SPEED);
  delay(800);
}

void avoidObstacleRight() {
  // Turn right and move forward
  rotateRight(TURN_SPEED);
  delay(600);
  moveForward(BASE_SPEED, BASE_SPEED);
  delay(800);
}

void searchForLine() {
  // Look for line by rotating slowly
  static unsigned long searchStartTime = 0;
  static bool searchDirection = true; // true = right, false = left
  static bool hasStartedSearch = false;
  
  if (!hasStartedSearch) {
    searchStartTime = millis();
    hasStartedSearch = true;
    Serial.println("Starting line search...");
  }
  
  // Check if line is found
  if (leftOnLine || rightOnLine) {
    Serial.println("Line found! Resuming line following");
    stopMotors();
    delay(200);
    hasStartedSearch = false;
    currentState = LINE_FOLLOWING;
    return;
  }
  
  // If searching for more than 3 seconds, try the other direction
  if (millis() - searchStartTime > 3000) {
    searchDirection = !searchDirection;
    searchStartTime = millis();
    Serial.println("Changing search direction");
  }
  
  // Search in current direction
  if (searchDirection) {
    rotateRight(SEARCH_SPEED);
  } else {
    rotateLeft(SEARCH_SPEED);
  }
  
  // If searching for too long (6 seconds total), move forward and try again
  if (millis() - searchStartTime > 6000) {
    Serial.println("Line search timeout, moving forward");
    moveForward(SEARCH_SPEED, SEARCH_SPEED);
    delay(500);
    hasStartedSearch = false;
    searchStartTime = millis();
  }
}

void executeLineFollowing() {
  // Immediate response to sensor changes - no delays!
  
  if (!leftOnLine && !rightOnLine) {
    // Both sensors off line - robot is centered, move forward
    moveForward(BASE_SPEED, BASE_SPEED);
    
  } else if (leftOnLine && !rightOnLine) {
    // Left sensor on line - robot drifted left, turn right immediately
    if (!rightSensorReliable) {
      gentleTurnRight();
    } else {
      turnRight();
    }
    
  } else if (!leftOnLine && rightOnLine) {
    // Right sensor on line - robot drifted right, turn left immediately  
    turnLeft();
    
  } else if (leftOnLine && rightOnLine) {
    // Both sensors on line - intersection, wide line, or perfect center
    moveForward(BASE_SPEED * 0.6, BASE_SPEED * 0.6);
  }
  
  // Additional logic: If right sensor seems stuck, compensate
  if (!rightSensorReliable && leftOnLine) {
    moveForwardWithBias(BASE_SPEED, 0.8, 1.0);
  }
}

void printDebugInfo() {
  Serial.print("State: ");
  switch(currentState) {
    case LINE_FOLLOWING: Serial.print("LINE_FOLLOW"); break;
    case OBSTACLE_DETECTED: Serial.print("OBSTACLE"); break;
    case SCANNING: Serial.print("SCANNING"); break;
    case AVOIDING_OBSTACLE: Serial.print("AVOIDING"); break;
    case SEARCHING_LINE: Serial.print("SEARCHING"); break;
  }
  
  Serial.print(" | Dist: ");
  Serial.print(currentDistance);
  Serial.print("cm | L:");
  Serial.print(leftOnLine ? "■" : "□");
  Serial.print(" R:");
  Serial.print(rightOnLine ? "■" : "□");
  
  if (!rightSensorReliable) {
    Serial.print(" [R-FAULT]");
  }
  
  Serial.println();
}

// === Motor Control Functions ===

void moveForward(int leftSpeed, int rightSpeed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, leftSpeed);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, rightSpeed);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, REVERSE_SPEED);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, REVERSE_SPEED);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, TURN_SPEED + 10);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, REVERSE_SPEED);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, REVERSE_SPEED);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, TURN_SPEED + 10);
}

void rotateRight(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed);
}

void rotateLeft(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void moveForwardWithBias(int baseSpeed, float leftBias, float rightBias) {
  int leftSpeed = baseSpeed * leftBias;
  int rightSpeed = baseSpeed * rightBias;
  moveForward(leftSpeed, rightSpeed);
}

void gentleTurnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, BASE_SPEED);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, BASE_SPEED * 0.3);
}

void gentleTurnLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, BASE_SPEED * 0.3);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, BASE_SPEED);
}
