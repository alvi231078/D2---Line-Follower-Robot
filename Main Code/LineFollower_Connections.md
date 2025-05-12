# Line Follower Car

This project is a simple Line Follower Car built using an Arduino Uno, two DC motors, an IR sensor, and an ultrasonic sensor. The car will follow a line based on the input from the IR sensors, and avoid obstacles with the help of the ultrasonic sensor.

## Components Used:

- Arduino Uno
- Motor Driver (L298N or similar)
- MG78MO5 Motor Controller
- 2 DC Motors
- SG90 Servo Motor
- 2 IR Sensors
- Ultrasonic Sensor (HC-SR04)
- Jumper Wires
- Breadboard

## Wiring Connections:

### Motor Driver (L298N or similar)
- **ENA** (Enable A): Connect to 5V.
- **ENB** (Enable B): Connect to 5V.
- **IN1**: Connect to Arduino pin **D3** (Control Pin 1 for Motor A).
- **IN2**: Connect to Arduino pin **D4** (Control Pin 2 for Motor A).
- **IN3**: Connect to Arduino pin **D5** (Control Pin 1 for Motor B).
- **IN4**: Connect to Arduino pin **D6** (Control Pin 2 for Motor B).
- **OUT1**: Connect to one terminal of Motor A.
- **OUT2**: Connect to the other terminal of Motor A.
- **OUT3**: Connect to one terminal of Motor B.
- **OUT4**: Connect to the other terminal of Motor B.
- **VCC**: Connect to **9V** from the battery or external power supply.
- **5V**: Connect to **5V** pin on the Arduino.
- **GND**: Connect to **GND** on Arduino.

### IR Sensors (Left and Right)
- **VCC**: Connect to **5V** on Arduino.
- **GND**: Connect to **GND** on Arduino.
- **OUT** (Signal Pin):
  - Left IR Sensor: Connect to Arduino pin **A0** (Analog input).
  - Right IR Sensor: Connect to Arduino pin **A1** (Analog input).

### Ultrasonic Sensor (HC-SR04)
- **VCC**: Connect to **5V** on Arduino.
- **GND**: Connect to **GND** on Arduino.
- **Trig**: Connect to Arduino pin **D7** (Trigger Pin).
- **Echo**: Connect to Arduino pin **D8** (Echo Pin).

### Servo Motor (SG90)
- **VCC**: Connect to **5V** on Arduino.
- **GND**: Connect to **GND** on Arduino.
- **Signal**: Connect to Arduino pin **D9**.

## Circuit Diagram:
![Circuit Diagram](circuit_diagram.png)

## Code Logic:

1. The IR sensors detect the black line on the surface and send signals to the Arduino.
2. The Arduino controls the motors based on the sensor input:
   - If both sensors detect the line, the car moves forward.
   - If the left sensor detects the line, the car turns right.
   - If the right sensor detects the line, the car turns left.
   - If both sensors lose the line, the car stops or searches for the line.
3. The ultrasonic sensor is used to detect obstacles, and if an obstacle is detected, the car will stop and avoid the obstacle.

## Arduino Code:

```cpp
#include <Servo.h>

const int leftSensor = A0;
const int rightSensor = A1;
const int motorA1 = 3;
const int motorA2 = 4;
const int motorB1 = 5;
const int motorB2 = 6;

const int trigPin = 7;
const int echoPin = 8;

Servo myservo;
long duration;
int distance;

void setup() {
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  myservo.attach(9);
  Serial.begin(9600);
}

void loop() {
  // Read sensors
  int leftState = analogRead(leftSensor);
  int rightState = analogRead(rightSensor);

  // Measure distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance < 10) {
    stopCar();  // Stop if obstacle is detected
  } else {
    if (leftState > 500 && rightState > 500) {
      moveForward();  // Move forward if both sensors detect the line
    } else if (leftState < 500) {
      turnRight();  // Turn right if the left sensor is off the line
    } else if (rightState < 500) {
      turnLeft();  // Turn left if the right sensor is off the line
    }
  }
}

void moveForward() {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
}

void turnRight() {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
}

void turnLeft() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
}

void stopCar() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
}
