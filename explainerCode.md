

### 📘 `EXPLAIN.md` – Line-by-Line Code Explanation

```markdown
# 📘 Code Explanation – Line-Following Robot with Obstacle Avoidance

This document explains each section of the Arduino code used for an intelligent robot that can follow lines and avoid obstacles using ultrasonic and IR sensors.

---

## 🔧 Pin Definitions

```cpp
#define motorPin1 9
#define motorPin2 10
#define motorPin3 5
#define motorPin4 6
```
- These pins control the two DC motors via an H-bridge (e.g., L298N).

```cpp
#define TRIG_PIN 7
#define ECHO_PIN 8
```
- Ultrasonic sensor pins: `TRIG` sends a pulse, `ECHO` receives it.

```cpp
#define IR_LEFT 2
#define IR_RIGHT 3
```
- Left and right IR sensors for line detection.

```cpp
#define OBSTACLE_DISTANCE_THRESHOLD 30   // cm
#define RECOVERY_TIMEOUT 3000            // ms
#define AVOID_TIMEOUT 5000               // ms
```
- Constants for obstacle distance threshold and recovery timeouts.

---

## 🏁 setup()

```cpp
void setup() {
  // Set motor, ultrasonic, and IR pins
  pinMode(...);
  Serial.begin(9600);
}
```
- Prepares all input/output pins.
- Starts serial monitor for debugging and logs.

---

## 🔄 loop()

```cpp
long distance = getDistance();
bool leftIR = digitalRead(IR_LEFT);
bool rightIR = digitalRead(IR_RIGHT);
```
- Reads obstacle distance and IR sensor values (`LOW` = black line).

### Logging

```cpp
Serial.print("Distance: ");
Serial.println(distance);
```
- Shows real-time sensor data in the Serial Monitor.

### Obstacle Check

```cpp
if (distance < OBSTACLE_DISTANCE_THRESHOLD) {
  avoidObstacle();
  return;
}
```
- If the robot sees an obstacle within 30 cm, it performs a detour.

### Line Following

```cpp
if (!leftIR && !rightIR) moveForward();
else if (!leftIR && rightIR) turnLeft();
else if (leftIR && !rightIR) turnRight();
else recoverLine();
```
- Line-following logic using 2 IR sensors.

---

## 📏 getDistance()

```cpp
digitalWrite(TRIG_PIN, HIGH);
long duration = pulseIn(ECHO_PIN, HIGH, 20000);
return duration * 0.034 / 2;
```
- Measures time taken for sound to bounce back and converts to distance.
- Includes timeout of 20 ms to avoid sensor hang.

---

## 🚗 Motor Functions

```cpp
moveForward()
stopMotors()
turnLeft()
turnRight()
```
- Control robot movement by setting motor pins HIGH or LOW.

---

## 🔀 avoidObstacle()

```cpp
turnRight(); delay(400);
moveForward(); check distance;
turnLeft(); delay(400);
moveForward(); delay(500);
recoverLine();
```
- Turns away from obstacle, moves forward beside it, then returns to line.
- Uses timeout to avoid getting stuck in obstacle avoidance mode.

---

## ♻️ recoverLine()

```cpp
spin right slowly until IR_LEFT or IR_RIGHT sees black
```
- Spins in place to search for the black line.
- Times out if line isn't found within `RECOVERY_TIMEOUT`.

---

## ✅ Built-In Safety & Reliability

- **Timeouts**: Prevent infinite loops (e.g. in line recovery).
- **Distance Thresholds**: Stops or avoids collisions.
- **Failsafe Spins**: Robot always tries to recover or move forward.

---

## 📚 Further Ideas

- Use 3 IR sensors (left, center, right) for smoother detection.
- Replace `delay()` with `millis()` for non-blocking movement.
- Add `analogWrite()` (PWM) for speed control.
- Add remote or app-based control override.

---
