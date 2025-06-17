#include "logic_module.h"

/**
 * Decides the robot's movement command based on IR sensor inputs.
 *
 * @param leftOnLine         True if the left IR sensor detects the black line.
 * @param rightOnLine        True if the right IR sensor detects the black line.
 * @param rightSensorReliable Flag indicating whether the right sensor is considered reliable.
 *
 * @return RobotCommand      An enum representing the robot's movement decision.
 */
RobotCommand decideLineFollowAction(bool leftOnLine, bool rightOnLine, bool rightSensorReliable) {
    // Case 1: Both sensors off the line (white surface) -> Move forward
    if (!leftOnLine && !rightOnLine) {
        return CMD_FORWARD;
    }

    // Case 2: Left on line, right off the line
    // If the right sensor is reliable, assume it's correct and turn right
    // If not, apply a right bias (softer turn)
    else if (leftOnLine && !rightOnLine) {
        return rightSensorReliable ? CMD_RIGHT : CMD_BIAS_RIGHT;
    }

    // Case 3: Right on line, left off the line -> Turn left
    else if (!leftOnLine && rightOnLine) {
        return CMD_LEFT;
    }

    // Case 4: Both sensors detect line -> Move forward (assume well-centered)
    else if (leftOnLine && rightOnLine) {
        return CMD_FORWARD;
    }

    // Fallback (should not occur) -> Stop
    return CMD_STOP;
}
