// === Motor pins: our robot's little legs 🦿🦿 ===
const int motorL1 = 9;    // Left forward
const int motorL2 = 10;   // Left backward
const int motorR1 = 5;    // Right forward
const int motorR2 = 6;    // Right backward

// === Ultrasonic sensor: the robot's eyes 👀 ===
const int trigPin = 7;    // "Hey! Echo this pulse!"
const int echoPin = 8;    // "Okay, I’ll time the bounce back."

// === IR sensors: to sniff the line like a bloodhound 🐶 ===
const int irLeft  = 2;    // Left paw
const int irRight = 3;    // Right paw

// === Some magical numbers that work well ✨ ===
const int obstacleThreshold = 30;      // If something’s closer than 30cm, PANIC!
const long recoveryTimeout  = 3000;    // Max time to search for the line before we give up 😢
const long avoidTimeout     = 5000;    // Max time we try to go around stuff

void setup() {
  // Wake up all the muscles and senses
  pinMode(motorL1, OUTPUT);
  pinMode(motorL2, OUTPUT);
  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);

  Serial.begin(9600); // Open our chat line 📡
}

void loop() {
  long distance = getDistance();     // "How close is the danger??"
  bool leftIR = digitalRead(irLeft);
  bool rightIR = digitalRead(irRight);

  // Share the latest gossip from our sensors
  reportStatus(distance, leftIR, rightIR);

  // Now make a wise decision 🤖
  if (distance < obstacleThreshold) {
    Serial.println("😱 Too close for comfort! Dodging...");
    avoid();
    return;
  }

  if (!leftIR && !rightIR) {
    Serial.println("😎 All good. Zooming ahead!");
    moveForward();
  } else if (!leftIR && rightIR) {
    Serial.println("↪️ Drifting right. Let's swing left.");
    turnLeft();
  } else if (leftIR && !rightIR) {
    Serial.println("↩️ Drifting left. Time to swing right.");
    turnRight();
  } else {
    Serial.println("🧐 Lost the line... Initiating detective mode.");
    recover();
  }

  delay(100); // Chill for a bit
}

// This function lets the robot measure distance like a bat 🦇
long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 20000); // Wait up to 20ms
  if (duration == 0) return 999;                 // “I heard nothing… Must be far away!”

  return duration * 0.034 / 2;                   // Convert echo time to distance
}

// Tells us what the robot sees and senses 🤓
void reportStatus(long dist, bool left, bool right) {
  Serial.print("📏 Distance: ");
  Serial.print(dist);
  Serial.println(" cm");

  Serial.print("IR Left: ");
  Serial.print(left ? "WHITE" : "BLACK");

  Serial.print(" | IR Right: ");
  Serial.println(right ? "WHITE" : "BLACK");
}

// Let's go straight like a train on tracks 🚂
void moveForward() {
  digitalWrite(motorL1, HIGH);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, HIGH);
  digitalWrite(motorR2, LOW);
}

// Halt! Take a deep breath 😮‍💨
void stopMotors() {
  digitalWrite(motorL1, LOW);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, LOW);
}

// Rotate to the left like you're dodging a water balloon 💦↩️
void turnLeft() {
  digitalWrite(motorL1, LOW);
  digitalWrite(motorL2, HIGH);
  digitalWrite(motorR1, HIGH);
  digitalWrite(motorR2, LOW);
}

// Spin right like you're grooving to the beat 🔁
void turnRight() {
  digitalWrite(motorL1, HIGH);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, HIGH);
}

// "Uh oh, I lost the trail!" — Spin until you find the line again 🕵️‍♂️
void recover() {
  stopMotors();
  delay(100); // Take a moment to panic

  // Start spinning to find the line again
  digitalWrite(motorL1, HIGH);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, HIGH);

  unsigned long start = millis();

  while (digitalRead(irLeft) == HIGH && digitalRead(irRight) == HIGH) {
    if (millis() - start > recoveryTimeout) {
      Serial.println("😭 Gave up... line is gone forever.");
      stopMotors();
      return;
    }
    delay(20); // Check again after a short nap
  }

  stopMotors();
  Serial.println("🎉 Woohoo! Found the line again!");
}

// This one is like a 5-step dance move to dodge trouble 🕺💃
void avoid() {
  unsigned long start = millis();

  // Step 1: Get outta there!
  turnRight();
  delay(400);

  // Step 2: Sneak past the obstacle
  moveForward();
  unsigned long moveTime = millis();
  while (millis() - moveTime < 1200) {
    if (getDistance() > obstacleThreshold + 10) break;
    delay(50);
  }

  // Step 3: Back on track
  turnLeft();
  delay(400);

  // Step 4: Move up a bit more
  moveForward();
  delay(500);

  // Step 5: Try to find the line again
  recover();

  if (millis() - start > avoidTimeout) {
    Serial.println("😤 Obstacle too stubborn! I'm going anyway.");
    moveForward();
    delay(1000);
  }
}
