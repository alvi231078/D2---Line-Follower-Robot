
---

## 🔋 1. Power Supply & Switch

| Component               | Connection                                     |                       |
| ----------------------- | ---------------------------------------------- | --------------------- |
| 9V Battery Positive (+) | → Switch input terminal                        |                       |
| Switch Output Terminal  | → Arduino VIN pin<br>→ L298N VCC (Motor Power) |                       |
| 9V Battery Negative (−) | → Common Ground                                | ([Circuit Digest][1]) |

> **Note:** The switch controls power to both the Arduino and the motor driver simultaneously.

---

## ⚙️ 2. Common Ground

Ensure all components share a common ground:

* Battery Negative (−)
* Arduino GND
* L298N GND
* All sensor GNDs (IR sensors, ultrasonic sensor, servo motor, color sensor)([Instructables][2], [How To Mechatronics][3])

> **Importance:** A common ground is crucial for proper operation and accurate sensor readings.

---

## 🧠 3. Arduino Uno Pin Connections

| Arduino Pin | Connected To                                                    |   |
| ----------- | --------------------------------------------------------------- | - |
| VIN         | Switch output terminal (from 9V battery)                        |   |
| GND         | Common Ground                                                   |   |
| D5          | L298N IN1                                                       |   |
| D6          | L298N IN2                                                       |   |
| D7          | L298N IN3                                                       |   |
| D8          | L298N IN4                                                       |   |
| D9          | Servo Motor Signal                                              |   |
| D10         | L298N ENA (PWM for Motor A)                                     |   |
| D11         | L298N ENB (PWM for Motor B)                                     |   |
| D12         | Ultrasonic Sensor Trig                                          |   |
| D13         | Ultrasonic Sensor Echo                                          |   |
| A0          | Color Sensor S0                                                 |   |
| A1          | Color Sensor S1                                                 |   |
| A2          | Color Sensor S2                                                 |   |
| A3          | Color Sensor S3                                                 |   |
| A4          | Color Sensor OUT (Digital or Analog Input, depending on sensor) |   |
| D3          | Left IR Sensor Signal (Digital Output)                          |   |
| D4          | Right IR Sensor Signal (Digital Output)                         |   |
| 5V          | Powers sensors and servo (from Arduino regulator)               |   |

> **Note:** IR sensors are now connected to digital pins D3 and D4.

---

## 🔌 4. L298N Motor Driver

| L298N Pin | Connected To                                 |                      |
| --------- | -------------------------------------------- | -------------------- |
| VCC       | Switch output terminal (9V battery positive) |                      |
| GND       | Common Ground                                |                      |
| ENA       | Arduino 5V               |                      |
| ENB       | Arduino 5V              |                      |
| IN1       | Arduino D5                                   |                      |
| IN2       | Arduino D6                                   |                      |
| IN3       | Arduino D7                                   |                      |
| IN4       | Arduino D8                                   |                      |
| OUT1      | Motor B terminal 1                           |                      |
| OUT2      | Motor B terminal 2                           |                      |
| OUT3      | Motor A terminal 1                           |                      |
| OUT4      | Motor A terminal 2                           | Motor B is left motor A is right |

> **Tip:** Connecting ENA and ENB to PWM-capable pins (D10 and D11) allows for motor speed control using `analogWrite()`.

---

## 📟 5. IR Sensors (Left & Right)

| IR Sensor Pin | Connected To                                          |                                    |
| ------------- | ----------------------------------------------------- | ---------------------------------- |
| VCC           | Arduino 5V                                            |                                    |
| GND           | Common Ground                                         |                                    |
| OUT (Signal)  | Left Sensor → Arduino D3<br>Right Sensor → Arduino D4 | ([Instructables][5], [YouTube][6]) |

> **Note:** IR sensors are connected to digital pins D3 and D4, providing digital output.

---

## 📏 6. Ultrasonic Sensor (HC-SR04)

| Ultrasonic Pin | Connected To  |                            |
| -------------- | ------------- | -------------------------- |
| VCC            | Arduino 5V    |                            |
| GND            | Common Ground |                            |
| Trig           | Arduino D12   |                            |
| Echo           | Arduino D13   | ([How To Mechatronics][3]) |

> **Tip:** Ensure that the Echo pin is connected to a digital pin capable of receiving input.

---

## 🔄 7. Servo Motor (SG90)

| Servo Pin | Connected To  |   |
| --------- | ------------- | - |
| VCC       | Arduino 5V    |   |
| GND       | Common Ground |   |
| Signal    | Arduino D9    |   |

> **Caution:** Servo motors can draw significant current. If you experience issues, consider powering the servo with an external 5V supply and connecting the grounds together.

---

## 🎨 8. Color Sensor (e.g., TCS3200)

| Color Sensor Pin | Connected To                                         |   |
| ---------------- | ---------------------------------------------------- | - |
| VCC              | Arduino 5V                                           |   |
| GND              | Common Ground                                        |   |
| S0               | Arduino A0                                           |   |
| S1               | Arduino A1                                           |   |
| S2               | Arduino A2                                           |   |
| S3               | Arduino A3                                           |   |
| OUT              | Arduino A4                                           |   |
| LED              | Arduino 5V (or Arduino pin via resistor for control) |   |

> **Note:** The OUT pin can be connected to a digital or analog pin to measure frequency output corresponding to color intensity.

---


