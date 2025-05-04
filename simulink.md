# 🚗 Line Follower Robot using MATLAB Simulink

This project demonstrates how to simulate and deploy an autonomous **line follower vehicle** using **MATLAB Simulink** and **Arduino Uno**. It uses **IR sensors** to detect the line path and controls the direction of two DC motors via **L298N Motor Driver**.


## 🛠 Hardware Used

* Arduino Uno
* 2 × DC Motors
* L298N Dual H-Bridge Motor Driver
* 2 × IR Sensors (Digital)
* Jumper wires, breadboard
* Optional: Ultrasonic sensor, TCS3200/TCS34725 (future)



## 🧱 Simulink Blocks Used

* **Digital Input** (IR\_LEFT: D2, IR\_RIGHT: D3)
* **Digital Output** (Motor Pins: D5, D6, D9, D10)
* **Logic Gates** (NOT, AND)
* **Multiport Switch**
* **Constant Blocks** (simulate logic levels)
* **Goto/From Tags** (signal routing)
* **Switch Block** (decision logic for motor direction)


## 🧠 Logic Overview

### IR Sensor Logic:

| IR\_LEFT | IR\_RIGHT | Action       |
| -------- | --------- | ------------ |
| 0        | 0         | Stop         |
| 0        | 1         | Turn Left    |
| 1        | 0         | Turn Right   |
| 1        | 1         | Move Forward |

### Motor Logic:

* Each motor has 2 input pins controlled through **multiport switches**.
* Based on the IR logic, control signals are routed to the motor inputs to drive the vehicle in the required direction.


## ✅ Current Features

* Complete **IR-based line-following logic**
* Simulates **four movement states**: Stop, Left, Right, Forward
* Multiport logic linked to motor pins:

  * Motor A: D9 (IN1), D10 (IN2)
  * Motor B: D5 (IN3), D6 (IN4)
* Built using modular **logic blocks** for learning and clarity


## 🧪 How to Test

1. Open the `line_follower_vehicle_Simulink.slx` in Simulink.
2. Ensure **Simulink Support Package for Arduino** is installed.
3. Set **hardware board** to "Arduino Uno" in Model Settings.
4. Use simulation or **Deploy to Hardware**.
5. Place the robot on a black line and observe motor reaction to IR input conditions.


## 🔮 Future Enhancements

* Add **Ultrasonic sensor** (`HC-SR04`) with **servo motor** for obstacle detection
* Add **Color Detection Sensor** (`TCS3200` or `TCS34725`) for object interaction
* Apply **PID control** for smoother turns
* Optimize logic using **Stateflow**
