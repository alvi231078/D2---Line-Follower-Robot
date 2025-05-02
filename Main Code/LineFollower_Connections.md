# Intelligent Line Follower & Obstacle Avoidance Robot

This robot combines IR-based line following, ultrasonic obstacle detection, and optional color detection using the TCS3200 color sensor. It is designed to continue operating even if the color sensor is not connected or active.

---

## 🔌 Wiring Connections

### 🟦 Arduino UNO Pins

| Component            | Arduino Pin | Description                          |
|---------------------|-------------|--------------------------------------|
| Left Motor          | 9           | Left motor forward (PWM capable)     |
|                     | 10          | Left motor backward                  |
| Right Motor         | 5           | Right motor forward (PWM capable)    |
|                     | 6           | Right motor backward                 |
| IR Sensor (Left)    | 2           | Digital input                        |
| IR Sensor (Right)   | 3           | Digital input                        |
| Ultrasonic Trigger  | 7           | Sends ultrasonic pulse               |
| Ultrasonic Echo     | 8           | Receives reflected pulse             |
| Color Sensor S0     | 4           | Frequency scaling control            |
| Color Sensor S1     | A0          | Frequency scaling control            |
| Color Sensor S2     | A1          | Color filter selection               |
| Color Sensor S3     | A2          | Color filter selection               |
| Color Sensor OUT    | A3          | Frequency output (to measure color)  |

---

## 🔋 Power Supply

- Motors must be powered with an external **6–12V battery** (Li-Po recommended).
- Arduino UNO can be powered via USB or from the **VIN pin** using the same battery.
- IR, ultrasonic, and color sensors operate on **5V from the Arduino**.

> ⚠️ Ensure your motor driver can handle the current load of your motors.

---

## 🧠 Behavior Summary

| Trigger Condition        | Robot Response                 |
|--------------------------|--------------------------------|
| Both IR sensors on black | Move forward                  |
| One IR sensor off line   | Adjust direction              |
| Both IR sensors on white | Line lost → Begin recovery    |
| Obstacle within 30 cm    | Avoid by turning and bypassing|
| Red color detected       | Stop temporarily              |
| Green color detected     | Move forward faster           |
| Blue color detected      | Turn right                    |
| No/Unknown color         | Ignore, continue normal logic |

---

## ✅ Notes

- The robot gracefully handles **missing or noisy color sensor input** by ignoring it.
- Behavior for colors can be **easily customized** in the Arduino code.
- Use the **Serial Monitor (9600 baud)** to view real-time sensor readings for debugging.
