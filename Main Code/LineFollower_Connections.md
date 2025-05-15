# Logic we will play on: 
Given that my sensors output:

0 when detecting a black line

1 when over a white surface

We can use this behavior to control your robot's movement:

Both sensors detect white (1): Robot moves forward.

Left sensor detects black (0), right sensor detects white (1): Robot turns left.

Left sensor detects white (1), right sensor detects black (0): Robot turns right.

Both sensors detect black (0): Robot stops or performs a specific action (e.g., U-turn).
# Line Follower Robot – Wiring 

## 1. Power Supply & Switch

| Component               | Connection                                     |
| ----------------------- | ---------------------------------------------- |
| 9V Battery Positive (+) | → Switch input terminal                        |
| Switch Output Terminal  | → Arduino VIN pin<br>→ L298N VCC (Motor Power) |
| 9V Battery Negative (−) | → Common Ground                                |

> **Note:** The switch controls power to both the Arduino and the motor driver simultaneously.

---

## 2. Common Ground

Ensure all components share a common ground:

* Battery Negative (−)
* Arduino GND
* L298N GND
* All sensor GNDs (IR sensors, ultrasonic sensor, servo motor, color sensor)

> **Importance:** A common ground is crucial for proper operation and accurate sensor readings.

---

## 3. Arduino Uno Pin Connections

| Arduino Pin | Connected To                                                    |
| ----------- | --------------------------------------------------------------- |
| VIN         | Switch output terminal (from 9V battery)                        |
| GND         | Common Ground                                                   |
| D3          | L298N IN1                                                       |
| D4          | L298N IN2                                                       |
| D5          | L298N IN3                                                       |
| D6          | L298N IN4                                                       |
| D7          | Ultrasonic Sensor Trig                                          |
| D8          | Ultrasonic Sensor Echo                                          |
| D9          | Servo Motor Signal                                              |
| A0          | Left IR Sensor Signal (Analog Output)                           |
| A1          | Right IR Sensor Signal (Analog Output)                          |
| D10         | Color Sensor S0                                                 |
| D11         | Color Sensor S1                                                 |
| D12         | Color Sensor S2                                                 |
| D13         | Color Sensor S3                                                 |
| D2 (or A2)  | Color Sensor OUT (Digital or Analog Input, depending on sensor) |
| 5V          | Powers sensors and servo (from Arduino regulator)               |

---

## 4. L298N Motor Driver

| L298N Pin | Connected To                                        |
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

> **Tip:** If you wish to control motor speed, connect ENA and ENB to PWM-capable pins (e.g., D10 and D11) and use `analogWrite()` in your code.

---

## 5. IR Sensors (Left & Right)

| IR Sensor Pin | Connected To                                          |
| ------------- | ----------------------------------------------------- |
| VCC           | Arduino 5V                                            |
| GND           | Common Ground                                         |
| OUT (Signal)  | Left Sensor → Arduino A0<br>Right Sensor → Arduino A1 |

> **Note:** If your IR sensors provide digital output, you can connect them to digital pins instead of analog pins.

---

## 6. Ultrasonic Sensor (HC-SR04)

| Ultrasonic Pin | Connected To  |
| -------------- | ------------- |
| VCC            | Arduino 5V    |
| GND            | Common Ground |
| Trig           | Arduino D7    |
| Echo           | Arduino D8    |

> **Tip:** Ensure that the Echo pin is connected to a digital pin capable of receiving input.

---

## 7. Servo Motor (SG90)

| Servo Pin | Connected To  |
| --------- | ------------- |
| VCC       | Arduino 5V    |
| GND       | Common Ground |
| Signal    | Arduino D9    |

> **Caution:** Servo motors can draw significant current. If you experience issues, consider powering the servo with an external 5V supply and connecting the grounds together.

---

## 8. Color Sensor (e.g., TCS3200)

| Color Sensor Pin | Connected To                                         |
| ---------------- | ---------------------------------------------------- |
| VCC              | Arduino 5V                                           |
| GND              | Common Ground                                        |
| S0               | Arduino D10                                          |
| S1               | Arduino D11                                          |
| S2               | Arduino D12                                          |
| S3               | Arduino D13                                          |
| OUT              | Arduino D2 (or A2)                                   |
| LED              | Arduino 5V (or Arduino pin via resistor for control) |

> **Note:** The OUT pin can be connected to a digital pin to measure frequency output corresponding to color intensity.

---

## Summary Tips

* **Power Distribution:** Using the switch to control power to both the Arduino and motor driver ensures synchronized operation.
* **Grounding:** A single common ground is essential for reliable sensor readings and motor control.
* **Voltage Levels:** Ensure that all components receive appropriate voltage levels as per their specifications.
* **Current Requirements:** Verify that the Arduino's 5V pin can supply sufficient current for all connected devices. If not, use external power sources where necessary.

