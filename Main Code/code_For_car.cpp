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
const int BASE_SPEED    = 80;   
const int TURN_SPEED    = 75;   
const int REVERSE_SPEED = 65;   

// Sensor interpretation (adjust based on your IR sensors)
const bool LINE_DETECTED_STATE = false;  // false = LOW means line detected

// Timing for responsiveness
const int LOOP_DELAY = 8;          // Very fast loop - 8ms
const int SENSOR_READ_INTERVAL = 2; // Read sensors every 2ms for better reliability

// Variables for non-blocking operation
unsigned long lastSensorRead = 0;
unsigned long lastDebugPrint = 0;
const int DEBUG_INTERVAL = 200;    // Print debug info every 200ms

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

  // Serial monitor
  Serial.begin(9600);
  Serial.println("Real-time Line Following Robot");
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
  
  // Execute line following logic immediately based on current sensor state
  executeLineFollowing();
  
  // Debug output at reasonable interval (don't flood serial)
  if (currentTime - lastDebugPrint >= DEBUG_INTERVAL) {
    printDebugInfo();
    lastDebugPrint = currentTime;
  }
  
  delay(LOOP_DELAY);  // Small delay for stability
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
    // If right sensor hasn't detected anything for 5 seconds, mark as unreliable
    rightSensorReliable = false;
  }
  
  // Update sample index
  sampleIndex = (sampleIndex + 1) % SENSOR_SAMPLES;
}

void executeLineFollowing() {
  // Immediate response to sensor changes - no delays!
  
  if (!leftOnLine && !rightOnLine) {
    // Both sensors off line - robot is centered, move forward
    moveForward(BASE_SPEED, BASE_SPEED);
    
  } else if (leftOnLine && !rightOnLine) {
    // Left sensor on line - robot drifted left, turn right immediately
    // If right sensor is unreliable, use gentler correction
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
    // Move forward but slower to maintain control
    moveForward(BASE_SPEED * 0.6, BASE_SPEED * 0.6);
  }
  
  // Additional logic: If right sensor seems stuck, compensate
  if (!rightSensorReliable && leftOnLine) {
    // Right sensor might be faulty, rely more on left sensor
    moveForwardWithBias(BASE_SPEED, 0.8, 1.0); // Slightly favor right motor
  }
}

void printDebugInfo() {
  Serial.print("L:");
  Serial.print(leftOnLine ? "■" : "□");
  Serial.print(" R:");
  Serial.print(rightOnLine ? "■" : "□");
  
  // Show sensor reliability
  if (!rightSensorReliable) {
    Serial.print(" [R-FAULT]");
  }
  
  Serial.print(" | ");
  
  // Show action being taken
  if (!leftOnLine && !rightOnLine) {
    Serial.println("FORWARD");
  } else if (leftOnLine && !rightOnLine) {
    Serial.println(rightSensorReliable ? "TURN RIGHT" : "GENTLE RIGHT");
  } else if (!leftOnLine && rightOnLine) {
    Serial.println("TURN LEFT");
  } else {
    Serial.println("BOTH ON LINE");
  }
  
  // Raw sensor values for debugging
  Serial.print("Raw sensors: L=");
  Serial.print(digitalRead(LEFT_SENSOR));
  Serial.print(" R=");
  Serial.println(digitalRead(RIGHT_SENSOR));
}

// === Optimized Motor Control Functions ===

void moveForward(int leftSpeed, int rightSpeed) {
  // Left motor forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, leftSpeed);

  // Right motor forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, rightSpeed);
}

void turnRight() {
  // More aggressive right turn for immediate response
  // Left motor forward at higher speed
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, TURN_SPEED + 10);

  // Right motor backward for sharp turn
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, REVERSE_SPEED);
}

void turnLeft() {
  // More aggressive left turn for immediate response
  // Left motor backward for sharp turn
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, REVERSE_SPEED);

  // Right motor forward at higher speed
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, TURN_SPEED + 10);
}

void rotateRight(int speed) {
  // Left motor forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);

  // Right motor backward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed);
}

void rotateLeft(int speed) {
  // Left motor backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);

  // Right motor forward
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

// === Advanced Functions for Better Control ===

void moveForwardWithBias(int baseSpeed, float leftBias, float rightBias) {
  // Fine-tune individual motor speeds for micro-corrections
  int leftSpeed = baseSpeed * leftBias;
  int rightSpeed = baseSpeed * rightBias;
  
  moveForward(leftSpeed, rightSpeed);
}

void gentleTurnRight() {
  // Gentler turn - just reduce right motor speed instead of reversing
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, BASE_SPEED);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, BASE_SPEED * 0.3);  // Slow down right motor
}

void gentleTurnLeft() {
  // Gentler turn - just reduce left motor speed instead of reversing
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, BASE_SPEED * 0.3);  // Slow down left motor

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, BASE_SPEED);
}
