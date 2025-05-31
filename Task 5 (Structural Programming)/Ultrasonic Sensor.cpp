// Ultrasonic sensor pins
const int TRIG_PIN = 9;
const int ECHO_PIN = 8;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);  // Optional: For viewing output on Serial Monitor
}

void loop() {
  long duration;
  float distance;

  
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo pin
  duration = pulseIn(ECHO_PIN, HIGH);

  // Convert to distance in cm
  distance = duration * 0.034 / 2;

  // Display the result
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(200);  // Short delay between readings
}
