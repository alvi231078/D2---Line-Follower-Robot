
# Line Follower Robot with Obstacle and Color Detection

This robot uses IR sensors to follow a line, an ultrasonic sensor to detect obstacles, and a TCS3200 color sensor to detect colored zones (red, green, blue). If the color sensor is not connected, the robot will still function using IR and ultrasonic sensors.

---

## 🔌 Wiring Connections

### 🟦 Arduino UNO Pins

| Component          | Pin on Arduino | Description                         |
|-------------------|----------------|-------------------------------------|
| Left Motor        | 9              | Left motor forward (PWM capable)    |
|                   | 10             | Left motor backward                 |
| Right Motor       | 5              | Right motor forward (PWM capable)   |
|                   | 6              | Right motor backward                |
| IR Sensor (Left)  | 2              | Digital input                       |
| IR Sensor (Right) | 3              | Digital input                       |
| Ultrasonic Sensor | 7              | Trigger pin                         |
|                   | 8              | Echo pin                            |
| Color Sensor S0   | 4              | Frequency scaling control           |
| Color Sensor S1   | A0             | Frequency scaling control           |
| Color Sensor S2   | A1             | Color filter selection              |
| Color Sensor S3   | A2             | Color filter selection              |
| Color Sensor OUT  | A3             | Frequency output                    |

---

## 🔋 Power Supply

- **Motors** should be powered using an external 6–12V battery (e.g., Li-Po).
- **Arduino UNO** can be powered via USB or VIN pin from the same battery.
- **IR, Ultrasonic, and Color Sensors** use 5V from the Arduino.

> ⚠️ Make sure your motor driver or motor control circuit can handle the motor current load safely.

---

## 🧠 Behavior Summary

| Sensor Triggered    | Robot Action              |
|---------------------|---------------------------|
| Both IR on black    | Moves forward             |
| One IR off line     | Turns to correct position |
| Obstacle detected   | Avoids by turning         |
| Red color detected  | Stops temporarily         |
| Green color detected| Boosts forward            |
| Blue color detected | Turns right               |
| No color            | Ignores and continues     |

---

## ✅ Notes

- If the color sensor is not connected or returns noisy readings, the robot ignores it.
- We can customize actions for each color in the code.
- SEE `Serial Monitor` (9600 baud) to view sensor readings and debug.

