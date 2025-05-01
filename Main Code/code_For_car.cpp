// motor pns (i think these are for left and right wheels)
#define motorPin1 9
#define motorPin2 10
#define motorPin3 5
#define motorPin4 6

// ultra sonic senser (used for measuring distance??)
#define TRIG_PIN 7
#define ECHO_PIN 8

// IR sensers for line follower — left & right
#define IR_LEFT 2
#define IR_RIGHT 3

// some safe values. don't know why 30 but it works ok


#define OBSTACLE_DISTANCE_THRESHOLD 30   // distance in cm I guess??
#define RECOVERY_TIMEOUT 3000            // 3 sec recovery time
#define AVOID_TIMEOUT 5000               // timeout for obstacle avoid... 5 sec

void setup() {
  // motor pins setup
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  // ultrasonic init
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // IR line sensors
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  Serial.begin(9600); // hope this   baud rate is ok
}

void loop() {
  long distance = getDistance();
  bool leftIR = digitalRead(IR_LEFT);   // white = 1, black = 0... right?
  bool rightIR = digitalRead(IR_RIGHT);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("IR Left: ");
  Serial.print(leftIR ? "WHITE" : "BLACK");
  Serial.print(" | IR Right: ");
  Serial.println(rightIR ? "WHITE" : "BLACK");

  if (distance < OBSTACLE_DISTANCE_THRESHOLD) {
    Serial.println("⚠️ Something close! I think it's an obstacle!\n");
    avoidObstacle();
    return;
  }

  if (!leftIR && !rightIR) {
    Serial.println("✅ Seems like we're good. go forward!\n");
    moveForward();
  }
  else if (!leftIR && rightIR) {
    Serial.println("↪ drifting? try going left.\n");
    turnLeft();
  }
  else if (leftIR && !rightIR) {
    Serial.println("↩ oops, maybe right turn is needed.\n");
    turnRight();
  }
  else {
    Serial.println(" ehhh where’s the line? searching...\n");
    recoverLine();
  }

  delay(100); // prevent jitter
}

// distance checking function with ultrasonic sensor
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 20000); // wait max 20ms
  if (duration == 0) return 999;  // no reading?? assume far away
  return duration * 0.034 / 2;    // calc d ist in cm, magic number
}

// all robot moves
void moveForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void stopMotors() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}

void turnLeft() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void turnRight() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
}

// try going around stuff
void avoidObstacle() {
  unsigned long start = millis();

  // turn right quickly
  turnRight();
  delay(400);

  // move next to it (hope this is enough distance)
  moveForward();
  unsigned long forwardStart = millis();
  while (millis() - forwardStart < 1200) {
    if (getDistance() > OBSTACLE_DISTANCE_THRESHOLD + 10) break; // safe now?
    delay(50); // wait a bit before next check
  }

  // face forward again (i think)
  turnLeft();
  delay(400);

  // final move to original path (?)
  moveForward();
  delay(500);

  // try to find the line again
  recoverLine();

  // too slow? just go on anyway
  if (millis() - start > AVOID_TIMEOUT) {
    Serial.println("❌ can't avoid it anymore. just go ahead\n");
    moveForward();
    delay(1000);
  }
}

// try to find line again by spinning around
void recoverLine() {
  stopMotors();
  delay(100); // just in case



  // spin in place (right side I thin k?)
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);

  unsigned long startTime = millis();
  while (digitalRead(IR_LEFT) == HIGH && digitalRead(IR_RIGHT) == HIGH) {
    if (millis() - startTime > RECOVERY_TIMEOUT) {
      Serial.println("❌ couldn't find the line... giving up 😞\n");
      stopMotors();
      return;
    }
    delay(20); // slow check
  }

  // yay! found it
  stopMotors();
  Serial.println("✅ Found the line again!\n");
}
