# Line Follower Robot with Motor Driver and IR Sensors

This project is a simple line follower robot using two digital IR sensors and a motor driver to control two DC motors (left and right). The robot follows a black line on a white surface by reading sensor inputs and controlling motor speed and direction accordingly.

---

## Hardware Connections

### Motor Driver Pins
| Pin | Description             |
|------|------------------------|
| 10   | ENA - PWM Motor A (Left Motor Speed Control)   |
| 11   | ENB - PWM Motor B (Right Motor Speed Control)  |
| 5    | IN1 - Motor A Input 1 (Left Motor Direction)   |
| 6    | IN2 - Motor A Input 2 (Left Motor Direction)   |
| 7    | IN3 - Motor B Input 1 (Right Motor Direction)  |
| 8    | IN4 - Motor B Input 2 (Right Motor Direction)  |

### IR Sensor Pins (Digital)
| Pin | Description       |
|------|------------------|
| 3    | Left IR Sensor    |
| 4    | Right IR Sensor   |

---

## Speed Settings

| Variable       | Value | Description                  |
|----------------|-------|------------------------------|
| `baseSpeed`    | 65    | Normal forward speed (PWM)    |
| `maxSpeed`     | 90    | Maximum speed (not used in code) |
| `reverseSpeed` | 60    | Speed when reversing          |

---

## How It Works

- The robot reads the state of two IR sensors (`leftSensorPin` and `rightSensorPin`).
- Both sensors LOW: Robot moves forward.
- Left sensor HIGH, right sensor LOW: Robot turns left.
- Left sensor LOW, right sensor HIGH: Robot turns right.
- Both sensors HIGH: Robot reverses to search for the line.

---

## Functions

- `moveForward(speed)`: Drives both motors forward at the specified speed.
- `turnLeft(speed)`: Stops left motor and runs right motor forward to turn left.
- `turnRight(speed)`: Runs left motor forward and stops right motor to turn right.
- `moveBackward(speed)`: Drives both motors backward at the specified speed.

---

## Usage

1. Connect your motors and IR sensors as described in the Hardware Connections.
2. Upload the provided Arduino sketch to your microcontroller.
3. Place the robot on a black line on a white surface and power it on.
4. The robot will follow the line by continuously adjusting its motor speeds based on sensor inputs.

---

## Notes

- Make sure your IR sensors are calibrated properly for the surface colors.
- Adjust `baseSpeed` and `reverseSpeed` values if needed for better line tracking.
- `maxSpeed` is defined but not used in the current code. You can modify the code to utilize it if required.

---

## License

This project is open source and free to use.
