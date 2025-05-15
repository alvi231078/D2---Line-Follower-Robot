

# Full Wiring Explanation with Power Switch for Line Follower Car

---

## 1. Power Supply and Switch

* **9V Battery Positive (+)** → **Switch input terminal**
* **Switch output terminal** →

  * **Arduino VIN pin** (to power Arduino via onboard regulator)
  * **L298N Motor Driver VCC (motor power input)**
* **9V Battery Negative (−)** → Connect to **Common Ground**

---

## 2. Common Ground (Very Important)

* Connect **Battery Negative (−)**, **Arduino GND**, **L298N GND**, **All sensors GND**, and **Color sensor GND** all together.
  This is the reference point for all signals and power.

---

## 3. Arduino Uno Pins

| Arduino Pin | Connects To                                                 |
| ----------- | ----------------------------------------------------------- |
| VIN         | Switch output terminal (from 9V battery)                    |
| GND         | Common Ground                                               |
| D3          | L298N Motor Driver IN1                                      |
| D4          | L298N Motor Driver IN2                                      |
| D5          | L298N Motor Driver IN3                                      |
| D6          | L298N Motor Driver IN4                                      |
| D7          | Ultrasonic Sensor Trig                                      |
| D8          | Ultrasonic Sensor Echo                                      |
| D9          | Servo Motor Signal                                          |
| A0          | Left IR Sensor Signal (analog output)                       |
| A1          | Right IR Sensor Signal (analog output)                      |
| 5V          | Power supply for sensors and servo (from Arduino regulator) |

---

## 4. L298N Motor Driver Pins

| L298N Pin | Connects To                                         |
| --------- | --------------------------------------------------- |
| VCC       | Switch output terminal (9V battery positive)        |
| GND       | Common Ground                                       |
| ENA       | Arduino 5V pin (or PWM pin if speed control needed) |
| ENB       | Arduino 5V pin (or PWM pin if speed control needed) |
| IN1       | Arduino D3                                          |
| IN2       | Arduino D4                                          |
| IN3       | Arduino D5                                          |
| IN4       | Arduino D6                                          |
| OUT1      | Motor A terminal 1                                  |
| OUT2      | Motor A terminal 2                                  |
| OUT3      | Motor B terminal 1                                  |
| OUT4      | Motor B terminal 2                                  |

---

## 5. IR Sensors (Left and Right)

| IR Sensor Pin | Connects To                                         |
| ------------- | --------------------------------------------------- |
| VCC           | Arduino 5V                                          |
| GND           | Common Ground                                       |
| OUT (Signal)  | Left sensor → Arduino A0; Right sensor → Arduino A1 |

---

## 6. Ultrasonic Sensor (HC-SR04)

| Ultrasonic Pin | Connects To   |
| -------------- | ------------- |
| VCC            | Arduino 5V    |
| GND            | Common Ground |
| Trig           | Arduino D7    |
| Echo           | Arduino D8    |

---

## 7. Servo Motor (SG90)

| Servo Pin | Connects To   |
| --------- | ------------- |
| VCC       | Arduino 5V    |
| GND       | Common Ground |
| Signal    | Arduino D9    |

---

## 8. Color Sensor (e.g., TCS3200)

| Color Sensor Pin | Connects To                                          |
| ---------------- | ---------------------------------------------------- |
| VCC              | Arduino 5V                                           |
| GND              | Common Ground                                        |
| S0               | Arduino digital pin (e.g., D10)                      |
| S1               | Arduino digital pin (e.g., D11)                      |
| S2               | Arduino digital pin (e.g., D12)                      |
| S3               | Arduino digital pin (e.g., D13)                      |
| OUT              | Arduino analog or digital pin (depends on sensor)    |
| LED              | Arduino 5V (or Arduino pin via resistor for control) |

---

# Summary Tips:

* The **switch breaks or connects the positive line from the battery** before powering Arduino and L298N.
* All grounds must be connected together—this is **crucial for correct sensor readings and motor control**.
* Arduino 5V pin powers sensors and servo; Arduino is powered via VIN pin from battery through the switch.
* Motor driver powers motors directly from 9V battery through the switch.

