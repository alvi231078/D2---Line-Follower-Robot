// IR sensor pins
const int LEFT_SENSOR  = 3;
const int RIGHT_SENSOR = 4;

void setup() {
  // Sensors
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);
}

void loop() {
  // Read IR sensors
  bool onLineL = digitalRead(LEFT_SENSOR)  == LOW;
  bool onLineR = digitalRead(RIGHT_SENSOR) == LOW;

  // Example use (optional)
  if (onLineL && onLineR) {
    // both sensors see line
  }
  else if (!onLineL && onLineR) {
    // left sensor off line
  }
  else if (onLineL && !onLineR) {
    // right sensor off line
  }
  else {
    // lost line completely
  }
}
