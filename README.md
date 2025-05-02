
# Intelligent Line Follower & Obstacle Avoidance Robot

This robot uses IR sensors to follow a line, an ultrasonic sensor to detect obstacles, and a TCS3200 color sensor to detect colored zones (red, green, blue). If the color sensor is not connected, the robot will still function using IR and ultrasonic sensors.

---

## 🛣 Line Following Logic

- **Sensor Input**:  
  - **Black surface** → `LOW`  
  - **White surface** → `HIGH`
  
- **Behavioral Responses**:
  - ➤ **Both sensors detect black** → Move straight
  - ➤ **One sensor detects white** → Adjust direction to stay on the line
  - ➤ **Both sensors detect white** → Line lost → Initiates recovery mode

---

## 🚧 Obstacle Avoidance Protocol

When an object is detected within **30-cm** using an ultrasonic sensor:

1. ➤ **Right Turn** – Exit the line path
2. ➤ **Forward Motion** – Navigate alongside the obstacle
3. ➤ **Left Turn** – Re-align with the original path
4. ➤ **Forward Motion** – Search and lock back onto the line

---

## 🎨 Color Sensor Behavior

If the optional TCS3200 color sensor is connected, the robot also responds to color:

- **Red** → Stop for 2 seconds  
- **Green** → Move forward faster  
- **Blue** → Turn right  
- **Unknown / No color** → Continue with normal behavior

> If the sensor is not connected, the robot continues using only IR and ultrasonic sensors.

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


---

## ⚙️ Key Features

- ⏱️ **Non-Blocking Timers** – Efficient use of `millis()` for delay-free recovery  
- 🪛 **Debug with Ease** – Real-time diagnostics via `Serial Monitor`  
- 📦 **Modular Codebase** – Clear, reusable, and well-documented functions  

---

## 👨‍💻 Contributors

| Name                        | Title                            | Institution                      |
|-----------------------------|----------------------------------|----------------------------------|
| **Md Ratul Ahmed Alvi**     | Electronics Engineering Student  | Hochschule Hamm-Lippstadt        |
| **Md Jubair Salehin Razin** | Electronics Engineering Student  | Hochschule Hamm-Lippstadt        |
| **Faysal Ahmed Tonmoy**     | Electronics Engineering Student  | Hochschule Hamm-Lippstadt        |
| **Mahin**                   | Electronics Engineering Student  | Hochschule Hamm-Lippstadt        |

