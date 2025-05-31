#ifndef MOTOR_MODULE_H
#define MOTOR_MODULE_H

// Motor pin definitions
#define ENA 10
#define ENB 11
#define IN1 5
#define IN2 6
#define IN3 7
#define IN4 8

// Speed constants (you can override from main)
#define BASE_SPEED 80
#define TURN_SPEED 75
#define REVERSE_SPEED 65

// Function declarations
void setupMotors();

void moveForward(int leftSpeed, int rightSpeed);
void moveBackward();
void turnLeft();
void turnRight();
void rotateLeft(int speed);
void rotateRight(int speed);
void stopMotors();
void gentleTurnRight();
void gentleTurnLeft();
void moveForwardWithBias(int baseSpeed, float leftBias, float rightBias);

#endif

