// ==== Pins ====

const int ML1 = 9;
const int ML2 = 10;
const int MR1 = 5;
const int MR2 = 6;

const int TRIG = 7;
const int ECHO = 8;

const int IR_L = 2;
const int IR_R = 3;

// ==== Constants ====

const int OBSTACLE_CM = 30;
const unsigned long RECOVER_MS = 3000;
const unsigned long AVOID_MS = 5000;

// ==== Setup ====

void setup() {
  pinMode(ML1, OUTPUT); pinMode(ML2, OUTPUT);
  pinMode(MR1, OUTPUT); pinMode(MR2, OUTPUT);

  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);
  pinMode(IR_L, INPUT); pinMode(IR_R, INPUT);

  Serial.begin(9600);
}

// ==== Loop ====

void loop() {
  long d = distance();
  bool l = digitalRead(IR_L);
  bool r = digitalRead(IR_R);

  Serial.print("Distance: "); Serial.print(d); Serial.println(" cm");
  Serial.print("IR L: "); Serial.print(l ? "WHITE" : "BLACK");
  Serial.print(" | IR R: "); Serial.println(r ? "WHITE" : "BLACK");

  if      (d < OBSTACLE_CM)    { Serial.println("⚠️ Obstacle"); avoid(); return; }
  else if (!l && !r)           { Serial.println("✅ Forward");  forward(); }
  else if (!l && r)            { Serial.println("↪ Left");     left(); }
  else if (l && !r)            { Serial.println("↩ Right");    right(); }
  else                         { Serial.println("❓ Recover");  recover(); }

  delay(100);
}

// ==== Distance ====

long distance() {
  digitalWrite(TRIG, LOW); delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long dur = pulseIn(ECHO, HIGH, 20000);
  return dur == 0 ? 999 : dur * 0.034 / 2;
}

// ==== Movement ====

void forward() {
  digitalWrite(ML1, HIGH); digitalWrite(ML2, LOW);
  digitalWrite(MR1, HIGH); digitalWrite(MR2, LOW);
}

void stopAll() {
  digitalWrite(ML1, LOW); digitalWrite(ML2, LOW);
  digitalWrite(MR1, LOW); digitalWrite(MR2, LOW);
}

void left() {
  digitalWrite(ML1, LOW);  digitalWrite(ML2, HIGH);
  digitalWrite(MR1, HIGH); digitalWrite(MR2, LOW);
}

void right() {
  digitalWrite(ML1, HIGH); digitalWrite(ML2, LOW);
  digitalWrite(MR1, LOW);  digitalWrite(MR2, HIGH);
}

// ==== Recovery ====

void recover() {
  stopAll(); delay(100);
  digitalWrite(ML1, HIGH); digitalWrite(ML2, LOW);
  digitalWrite(MR1, LOW);  digitalWrite(MR2, HIGH);

  unsigned long start = millis();
  while (digitalRead(IR_L) && digitalRead(IR_R)) {
    if (millis() - start > RECOVER_MS) {
      Serial.println("❌ No line");
      stopAll();
      return;
    }
    delay(20);
  }

  stopAll();
  Serial.println("✅ Line found");
}

// ==== Avoidance ====

void avoid() {
  unsigned long start = millis();

  right(); delay(400);
  forward();
  unsigned long fStart = millis();
  while (millis() - fStart < 1200) {
    if (distance() > OBSTACLE_CM + 10) break;
    delay(50);
  }

  left(); delay(400);
  forward(); delay(500);
  recover();

  if (millis() - start > AVOID_MS) {
    Serial.println("❌ Giving up — continue");
    forward(); delay(1000);
  }
}
