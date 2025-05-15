const int leftSensor = 2;  // Left IR sensor connected to digital pin D2
const int rightSensor = 3; // Right IR sensor connected to digital pin D3

void setup() {
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);
  Serial.begin(9600);
}

void loop() {
  int leftState = digitalRead(leftSensor);
  int rightState = digitalRead(rightSensor);

  Serial.print("Left Sensor: ");
  Serial.print(leftState);
  Serial.print(" | Right Sensor: ");
  Serial.println(rightState);

  delay(200);
}
