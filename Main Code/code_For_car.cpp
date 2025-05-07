// === Motor Controller Pins ===
const int motorL1 = 9;
const int motorL2 = 10;
const int motorR1 = 5;
const int motorR2 = 6;
const int motorSpeed = 150;

// === 3-Pin IR Sensor Outputs ===
const int irLeft = 2;   // OUT pin from left IR sensor
const int irRight = 3;  // OUT pin from right IR sensor

// === TCS3200 Color Sensor Pins (optional) ===
const int S0 = 4;
const int S1 = A0;
const int S2 = A1;
const int S3 = A2;
const int sensorOut = A3;

// === Color logic (optional) ===
void setup() {
  pinMode(motorL1, OUTPUT); pinMode(motorL2, OUTPUT);
  pinMode(motorR1, OUTPUT); pinMode(motorR2, OUTPUT);

  pinMode(irLeft, INPUT); pinMode(irRight, INPUT);

  pinMode(S0, OUTPUT); pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT); pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  digitalWrite(S0, HIGH); digitalWrite(S1, LOW); // 20% scaling

  Serial.begin(9600);
}

void loop() {
  bool leftIR = isLineUnderSensor(irLeft);
  bool rightIR = isLineUnderSensor(irRight);
  String color = readColor();

  logStatus(leftIR, rightIR, color);

  // Color-based actions (optional)
  if (color == "RED") {
    Serial.println("[COLOR] RED → Stopping");
    stopMotors();
    delay(2000);
    return;
  } else if (color == "GREEN") {
    Serial.println("[COLOR] GREEN → Boosting forward");
    moveForward();
    delay(800);
    return;
  } else if (color == "BLUE") {
    Serial.println("[COLOR] BLUE → Turning right");
    turnRight();
    delay(500);
    return;
  }

  // Line-following logic (IR-based only)
  if (!leftIR && !rightIR) {
    Serial.println("[✓] On line. Moving forward.");
    moveForward();
  } else if (!leftIR && rightIR) {
    Serial.println("[→] Adjusting left.");
    turnLeft();
  } else if (leftIR && !rightIR) {
    Serial.println("[←] Adjusting right.");
    turnRight();
  } else {
    Serial.println("[X] Line lost. Stopping.");
    stopMotors();
  }

  delay(100);
}

// === Movement ===
void moveForward() {
  analogWrite(motorL1, motorSpeed); digitalWrite(motorL2, LOW);
  analogWrite(motorR1, motorSpeed); digitalWrite(motorR2, LOW);
}

void stopMotors() {
  digitalWrite(motorL1, LOW); digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW); digitalWrite(motorR2, LOW);
}

void turnLeft() {
  digitalWrite(motorL1, LOW); analogWrite(motorL2, motorSpeed);
  analogWrite(motorR1, motorSpeed); digitalWrite(motorR2, LOW);
}

void turnRight() {
  analogWrite(motorL1, motorSpeed); digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW); analogWrite(motorR2, motorSpeed);
}

// === IR Sensor ===
bool isLineUnderSensor(int pin) {
  int val = digitalRead(pin);
  return val == LOW;  // LOW = black line
}

// === Color Sensor (optional) ===
String readColor() {
  int red = readColorFrequency(LOW, LOW);
  int green = readColorFrequency(HIGH, HIGH);
  int blue = readColorFrequency(LOW, HIGH);

  if (red < 0 || green < 0 || blue < 0) return "UNKNOWN";

  Serial.print("R: "); Serial.print(red);
  Serial.print(" G: "); Serial.print(green);
  Serial.print(" B: "); Serial.println(blue);

  if (red < green && red < blue) return "RED";
  if (green < red && green < blue) return "GREEN";
  if (blue < red && blue < green) return "BLUE";
  return "UNKNOWN";
}

int readColorFrequency(bool s2Val, bool s3Val) {
  digitalWrite(S2, s2Val); digitalWrite(S3, s3Val);
  delay(50);
  long pulse = pulseIn(sensorOut, LOW, 50000);
  return (pulse == 0) ? -1 : pulse;
}

// === Debugging ===
void logStatus(bool left, bool right, String color) {
  Serial.print("IR L: "); Serial.print(left ? "BLACK" : "WHITE");
  Serial.print(" | IR R: "); Serial.print(right ? "BLACK" : "WHITE");
  Serial.print(" | Color: "); Serial.println(color);
}
