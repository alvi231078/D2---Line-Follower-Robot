void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 bps
  pinMode(A0, INPUT); // Left IR sensor
  pinMode(A1, INPUT); // Right IR sensor
}

void loop() {
  int leftIR = analogRead(A0);  // Read value from left IR sensor
  int rightIR = analogRead(A1); // Read value from right IR sensor

  // Print the sensor values to the Serial Monitor
  Serial.print("Left IR: ");
  Serial.print(leftIR);
  Serial.print(" | Right IR: ");
  Serial.println(rightIR);

  delay(200); // Wait for 200 milliseconds before the next reading
}
