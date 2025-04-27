// Motor Pin Definitions
#define motorPin1 9    // Motor 1 control pin (IN1 on L293D)
#define motorPin2 10   // Motor 2 control pin (IN2 on L293D)
#define motorPin3 5    // Motor 3 control pin (IN3 on L293D)
#define motorPin4 6    // Motor 4 control pin (IN4 on L293D)

// Ultrasonic Pin Definitions
#define TRIG_PIN 7   // Trigger pin for Ultrasonic sensor
#define ECHO_PIN 8   // Echo pin for Ultrasonic sensor

void setup() {
  // Initialize Motor Control Pins
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  
  // Initialize Ultrasonic Sensor Pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(9600); // For debugging, view distance values

  // Start motors forward to test
  moveForward();
  delay(2000); // Run motors forward for 2 seconds

  stopMotors();
}

void loop() {
  // Measure Distance with Ultrasonic Sensor (for testing)
  long distance = getDistance();
  
  // Debugging output for distance
  Serial.print("Distance: ");
  Serial.println(distance);

  // If the distance is less than 20 inches (about 50.8 cm), stop the motors
  if (distance < 50.8) {
    stopMotors();  // Stop the motors if an obstacle is within range
  } else {
    moveForward(); // Otherwise, move forward
  }
}

// Function to get distance from Ultrasonic Sensor
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);  // Ensure the trigger pin is low
  delayMicroseconds(2); 

  digitalWrite(TRIG_PIN, HIGH);  // Send the pulse
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);  // Measure the pulse duration
  return duration * 0.034 / 2; // Convert the time into distance (cm)
}

// Function to move the car forward
void moveForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

// Function to stop the motors
void stopMotors() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}
