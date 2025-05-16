// Motor driver pins
const int ENA = 10;  // PWM Motor A (left)
const int ENB = 11;  // PWM Motor B (right)
const int IN1 = 5;   // Motor A input 1
const int IN2 = 6;   // Motor A input 2
const int IN3 = 7;   // Motor B input 1
const int IN4 = 8;   // Motor B input 2

// IR sensor pins (digital)
const int leftSensorPin = 3;
const int rightSensorPin = 4;

// Speed settings (PWM 0-255)
const int baseSpeed = 75;        // Base speed for moving forward
const int maxSpeed = 100;        // Maximum allowed speed

// PID control variables
float Kp = 25;                   // Proportional gain
float Ki = 0.2;                  // Integral gain
float Kd = 15;                   // Derivative gain
int lastError = 0;               // Last calculated error
float integral = 0;              // Integral term

// Timing
unsigned long lastTime = 0;      // Last correction time for derivative calculation

void setup() {
  Serial.begin(9600);
  
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  pinMode(leftSensorPin, INPUT);
  pinMode(rightSensorPin, INPUT);
  
  // Ensure motors are stopped at startup
  stopMotors();
  
  delay(1000);  // Short delay before starting
}

void loop() {
  // Read sensors (LOW means line detected)
  bool leftDetected = (digitalRead(leftSensorPin) == LOW);
  bool rightDetected = (digitalRead(rightSensorPin) == LOW);
  
  // Calculate time elapsed since last correction
  unsigned long currentTime = millis();
  float deltaTime = (currentTime - lastTime) / 1000.0; // Convert to seconds
  lastTime = currentTime;
  
  // Limit deltaTime to avoid huge jumps when timer overflows
  if (deltaTime > 1.0) deltaTime = 0.1;
  
  // Calculate position error (-1: far left, 0: centered, 1: far right)
  int error = 0;
  
  if (leftDetected && rightDetected) {
    // Both sensors on the line - go straight
    error = 0;
  } else if (leftDetected && !rightDetected) {
    // Left sensor on line, right sensor off - line is to the left
    error = -1;
  } else if (!leftDetected && rightDetected) {
    // Right sensor on line, left sensor off - line is to the right
    error = 1;
  } else {
    // Both sensors off line - use last error but increase it to make sharper turn
    error = lastError * 2;
    
    // If we've completely lost the line, implement recovery behavior
    if (lastError == 0) {
      // Rotate in place to find the line
      rotateToFindLine();
      return;
    }
  }
  
  // PID calculation
  integral = integral + error * deltaTime;
  
  // Limit integral term to prevent windup
  integral = constrain(integral, -10, 10);
  
  // Calculate derivative
  float derivative = (error - lastError) / deltaTime;
  
  // Calculate motor adjustment using PID
  float motorAdjustment = Kp * error + Ki * integral + Kd * derivative;
  
  // Save current error for next iteration
  lastError = error;
  
  // Calculate motor speeds
  int leftMotorSpeed = baseSpeed - motorAdjustment;
  int rightMotorSpeed = baseSpeed + motorAdjustment;
  
  // Constrain speeds to valid range with a minimum that ensures movement
  leftMotorSpeed = constrain(leftMotorSpeed, -maxSpeed, maxSpeed);
  rightMotorSpeed = constrain(rightMotorSpeed, -maxSpeed, maxSpeed);
  
  // Debug output
  Serial.print("Left: ");
  Serial.print(leftDetected);
  Serial.print(" Right: ");
  Serial.print(rightDetected);
  Serial.print(" Error: ");
  Serial.print(error);
  Serial.print(" Adjustment: ");
  Serial.print(motorAdjustment);
  Serial.print(" Motors L/R: ");
  Serial.print(leftMotorSpeed);
  Serial.print("/");
  Serial.println(rightMotorSpeed);
  
  // Set motor directions and speeds
  setMotors(leftMotorSpeed, rightMotorSpeed);
  
  // Small delay for stability
  delay(10);
}

// Set both motors with appropriate direction and speed
void setMotors(int leftSpeed, int rightSpeed) {
  // Left motor
  if (leftSpeed >= 0) {
    // Forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, leftSpeed);
  } else {
    // Backward
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, -leftSpeed);
  }
  
  // Right motor
  if (rightSpeed >= 0) {
    // Forward
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, rightSpeed);
  } else {
    // Backward
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, -rightSpeed);
  }
}

// Stop both motors
void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// Recovery behavior when both sensors are off the line
void rotateToFindLine() {
  // Determine rotation direction based on the last known error
  if (lastError < 0) {
    // Line was to the left, so rotate left
    setMotors(-60, 60);
  } else {
    // Line was to the right, so rotate right
    setMotors(60, -60);
  }
  
  // Continue rotating until one sensor finds the line
  while (digitalRead(leftSensorPin) == HIGH && digitalRead(rightSensorPin) == HIGH) {
    delay(5);
  }
  
  // Once line is found, reset integral to avoid overcompensation
  integral = 0;
}