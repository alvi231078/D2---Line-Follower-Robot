#ifndef LOGIC_MODULE_H
#define LOGIC_MODULE_H

// Commands for robot actions
enum RobotCommand {
  CMD_FORWARD,
  CMD_LEFT,
  CMD_RIGHT,
  CMD_STOP,
  CMD_BIAS_RIGHT
};

// Function to decide robot movement based on sensor input
RobotCommand decideLineFollowAction(bool leftOnLine, bool rightOnLine, bool rightSensorReliable);

#endif

