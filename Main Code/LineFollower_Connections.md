Great! Since you're using a **motor controller** (most likely an **L298N**, **L9110**, or **TB6612FNG**), here's an updated and complete version of your **`connection.md`** file tailored for that setup:

---

# 🛠️ Robot Sensor & Motor Controller Connections (`connection.md`)

## ⚡ Motor Controller Module

### Example: L298N or TB6612FNG

Controls two DC motors with direction and optional speed via PWM.

| Function           | Arduino Pin             | Notes                                  |
| ------------------ | ----------------------- | -------------------------------------- |
| Left Motor IN1     | 9                       | `motorL1` – Direction control          |
| Left Motor IN2     | 10                      | `motorL2` – Direction control          |
| Right Motor IN1    | 5                       | `motorR1` – Direction control          |
| Right Motor IN2    | 6                       | `motorR2` – Direction control          |
| Motor VCC          | External                | Connect to motor battery (e.g., 6–12V) |
| Motor GND          | Common GND              | Shared with Arduino GND                |
| 5V Output (opt.)   | Optional                | Can power Arduino if jumper is in      |
| ENA (if available) | PWM (or bridged to VCC) | Controls left motor speed              |
| ENB (if available) | PWM (or bridged to VCC) | Controls right motor speed             |

> 💡 If your module uses **ENA/ENB**, connect them to PWM pins or jumper them high to always enable the motors.

---

## 🔍 IR Line Sensors

| Sensor   | Arduino Pin | Type    | Notes                  |
| -------- | ----------- | ------- | ---------------------- |
| Left IR  | 2           | Digital | Returns `LOW` on black |
| Right IR | 3           | Digital | Returns `LOW` on black |

---

## 📏 Ultrasonic Distance Sensor (HC-SR04)

| Function | Arduino Pin | Direction | Notes         |
| -------- | ----------- | --------- | ------------- |
| Trigger  | 7           | Output    | Sends signal  |
| Echo     | 8           | Input     | Receives echo |

---

## 🎨 TCS3200 Color Sensor

| Pin Name | Arduino Pin | Direction | Description                   |
| -------- | ----------- | --------- | ----------------------------- |
| S0       | 4           | Output    | Frequency scaling             |
| S1       | A0          | Output    | Frequency scaling             |
| S2       | A1          | Output    | Color filter select           |
| S3       | A2          | Output    | Color filter select           |
| OUT      | A3          | Input     | Frequency output (color data) |

* **Frequency Scaling Configuration**:

  ```cpp
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW); // 20% output scaling
  ```

---

## ⚙️ System Thresholds & Timers

| Name                | Value   | Description                         |
| ------------------- | ------- | ----------------------------------- |
| `obstacleThreshold` | 30 cm   | Minimum distance before avoiding    |
| `recoveryTimeout`   | 3000 ms | Time to try recovering line         |
| `avoidTimeout`      | 5000 ms | Max time to complete obstacle avoid |
| `stuckTimeout`      | 4000 ms | Time before considered stuck        |

---

## 🖥️ Serial Monitor Debug Info

* **Baud Rate**: `9600`
* **Logs**:

  * IR sensor status (`BLACK`/`WHITE`)
  * Distance (Ultrasonic)
  * Detected color (TCS3200)
  * Motor actions (Forward, Left, Right, Stop)
  * Sensor and recovery errors

---

Would you like this as a downloadable `.md` file or a wiring diagram for your motor controller included as well?
