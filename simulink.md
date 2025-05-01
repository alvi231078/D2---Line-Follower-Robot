# 🚗 DC Motor Control with Arduino Using Simulink

This project demonstrates basic DC motor control using Simulink and an Arduino Uno. The motors are controlled via digital output pins connected to a dual H-Bridge driver (like L298N). Logic signals simulate simple movement modes like **forward** and **stop**.

## 🛠 Hardware Used
- Arduino Uno
- DC Motors (x2)
- L298N Driver
- External power supply (9V battery)
- Jumper wires, breadboard

## 📦 Simulink Blocks Used
- Constant
- Digital Output (from Arduino Support Package)
- Manual switch (to toggle logic)
- Hardware implementation set for Arduino Uno

## 🧠 Current Features
- Controls 2 DC motors using pins:
  - Motor A: D9 (IN1), D10 (IN2)
  - Motor B: D5 (IN3), D6 (IN4)
- Constant blocks simulate direction control
- Ready for real hardware deployment via **Deploy to Hardware**

## 🔮 Planned Features
- Add **IR Sensor** for line following (D2, D3)
- Add **Ultrasonic Sensor** (HC-SR04) with **Servo Motor** for obstacle detection
- Add **Color Detection Sensor** (TCS3200 or TCS34725) for object recognition
- Use logic blocks to switch between behaviors based on sensor inputs


## 📁 File
- DC_Motor_Control_Arduino.slx` — Matlab Simulink
