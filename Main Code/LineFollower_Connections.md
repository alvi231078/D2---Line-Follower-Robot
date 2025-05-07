

## 🔌 CONNECTIONS.md

### Microcontroller: **Arduino Uno**

---

### 🛞 Motor Driver (L298N or similar)

| Component       | Arduino Pin                               | Description |
| --------------- | ----------------------------------------- | ----------- |
| Motor Left IN1  | D9                                        | `motorL1`   |
| Motor Left IN2  | D10                                       | `motorL2`   |
| Motor Right IN1 | D5                                        | `motorR1`   |
| Motor Right IN2 | D6                                        | `motorR2`   |
| ENA / ENB (PWM) | Connected to 5V or PWM enabled via jumper |             |

---

### 📟 IR Sensors

| Sensor   | Arduino Pin | Description |
| -------- | ----------- | ----------- |
| Left IR  | D2          | `irLeft`    |
| Right IR | D3          | `irRight`   |

* IR sensors give LOW when line is detected (black surface).

---

### 📏 Ultrasonic Sensor (HC-SR04)

| Pin  | Arduino Pin | Description |
| ---- | ----------- | ----------- |
| VCC  | 5V          | Power       |
| GND  | GND         | Ground      |
| TRIG | D7          | `trigPin`   |
| ECHO | D8          | `echoPin`   |

---

### 🎨 TCS3200 Color Sensor

| Pin | Arduino Pin | Description         |
| --- | ----------- | ------------------- |
| VCC | 5V          | Power               |
| GND | GND         | Ground              |
| S0  | D4          | Frequency scaling   |
| S1  | A0          | Frequency scaling   |
| S2  | A1          | Color filter select |
| S3  | A2          | Color filter select |
| OUT | A3          | Frequency output    |

---

### 🔄 Servo Motor

| Wire   | Arduino Pin               | Description |
| ------ | ------------------------- | ----------- |
| Signal | D11                       | `servoPin`  |
| VCC    | 5V (external recommended) | Power       |
| GND    | GND                       | Ground      |

**Note:** Servo motors can draw a lot of current. If unstable, use an external 5V power supply and **connect GND of Arduino to GND of external power**.

---

### ✅ Power Supply Tips

* Use **external power** (e.g. 6V battery pack or 7.4V Li-ion) for motors.
* Use **5V regulator or BEC** if powering servo and sensors from battery.
* Always share **common GND** between Arduino, sensors, and motor driver.


