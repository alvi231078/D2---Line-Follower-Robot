#include "logic_module.h"

RobotCommand decideLineFollowAction(bool leftOnLine, bool rightOnLine, bool rightSensorReliable) {
  if (!leftOnLine && !rightOnLine) {
    return CMD_FORWARD;
  } 
  else if (leftOnLine && !rightOnLine) {
    return rightSensorReliable ? CMD_RIGHT : CMD_BIAS_RIGHT;
  } 
  else if (!leftOnLine && rightOnLine) {
    return CMD_LEFT;
  } 
  else if (leftOnLine && rightOnLine) {
    return CMD_FORWARD;
  }

  return CMD_STOP; // fallback
}
