// === Pin assignments ===

const int motorL1 = 9;
const int motorL2 = 10;
const int motorR1 = 5;
const int motorR2 = 6;

const int trigPin = 7;
const int echoPin = 8;

const int irLeft  = 2;
const int irRight = 3;

// === Constants ===

const int distThreshold = 30;         // cm
const long recoveryTimeout = 3000;    // ms
const long avoidTimeout    = 5000;    // ms

void setup() {
  pinMode(motorL1, OUTPUT); pinMode(motorL2, OUTPUT);
  pinMode(motorR1, OUTPUT); pinMode(motorR2, OUTPUT);

  pinMode(trigPin, OUTPUT); pinMode(echoPin, INPUT);
  pinMode(irLeft, INPUT);   pinMode(irRight, INPUT);

  Serial.begin(9600);
}

void loop() {
  long dist = getDistance();
  bool irL = digitalRead(irLeft);
  bool irR = digitalRead(irRight);

  logStatus(dist, irL, irR);

  if      (dist < distThreshold) { Serial.println("⚠️ Obstacle detected"); avoid(); return; }
  else if (!irL && !irR)         { Serial.println("✅ Forward"); moveForward(); }
  else if (!irL && irR)          { Serial.println("↪ Turning left"); turnLeft(); }
  else if (irL && !irR)          { Serial.println("↩ Turning right"); turnRight(); }
  else                           { Serial.println("❓ Searching for line"); recover(); }

  delay(100);
}

long getDistance() {
  digitalWrite(trigPin, LOW);  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 20000);
  return duration == 0 ? 999 : duration * 0.034 / 2;
}

void logStatus(long d, bool l, bool r) {
  Serial.print("Distance: "); Serial.print(d); Serial.println(" cm");
  Serial.print("IR L: "); Serial.print(l ? "WHITE" : "BLACK");
  Serial.print(" | IR R: "); Serial.println(r ? "WHITE" : "BLACK");
}

void moveForward() {
  digitalWrite(motorL1, HIGH); digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, HIGH); digitalWrite(motorR2, LOW);
}

void stopMotors() {
  digitalWrite(motorL1, LOW);  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);  digitalWrite(motorR2, LOW);
}

void turnLeft() {
  digitalWrite(motorL1, LOW);  digitalWrite(motorL2, HIGH);
  digitalWrite(motorR1, HIGH); digitalWrite(motorR2, LOW);
}

void turnRight() {
  digitalWrite(motorL1, HIGH); digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);  digitalWrite(motorR2, HIGH);
}

void recover() {
  stopMotors(); delay(100);

  digitalWrite(motorL1, HIGH); digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);  digitalWrite(motorR2, HIGH);

  unsigned long start = millis();
  while (digitalRead(irLeft) == HIGH && digitalRead(irRight) == HIGH) {
    if (millis() - start > recoveryTimeout) {
      Serial.println("❌ Could not find the line");
      stopMotors();
      return;
    }
    delay(20);
  }

  stopMotors();
  Serial.println("✅ Line found");
}

void avoid() {
  unsigned long tStart = millis();

  turnRight();   delay(400);
  moveForward();

  unsigned long moveStart = millis();
  while (millis() - moveStart < 1200) {
    if (getDistance() > distThreshold + 10) break;
    delay(50);
  }

  turnLeft();    delay(400);
  moveForward(); delay(500);
  recover();

  if (millis() - tStart > avoidTimeout) {
    Serial.println("❌ Timeout. Moving on.");
    moveForward(); delay(1000);
  }
}
