
// === PID Variables ===
float Kp = 25;
float Ki = 0.01;
float Kd = 10;

int error = 0;
float integral = 0;
int previous_error = 0;

// === Updated executeLineFollowing() function using PID ===
void executeLineFollowing() {
  // Step 1: Calculate error based on IR sensor states
  if (leftOnLine && !rightOnLine) {
    error = -1;  // drifted to the right
  } else if (!leftOnLine && rightOnLine) {
    error = 1;   // drifted to the left
  } else if (!leftOnLine && !rightOnLine) {
    error = 2;   // completely lost the line
  } else {
    error = 0;   // centered
  }

  // Step 2: PID calculation
  integral += error;
  int derivative = error - previous_error;
  int correction = Kp * error + Ki * integral + Kd * derivative;
  previous_error = error;

  // Step 3: Adjust motor speeds based on correction
  int leftSpeed = BASE_SPEED - correction;
  int rightSpeed = BASE_SPEED + correction;

  // Make sure speed stays within safe PWM range
  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  // Step 4: Move the motors with adjusted speeds
  moveForward(leftSpeed, rightSpeed);
}
