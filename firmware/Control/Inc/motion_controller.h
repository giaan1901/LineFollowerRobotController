#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H

#include "motor.h"

typedef struct {
    float full_command_mps; /* measured speed at command 1; >0 */
    float output_limit; /* (0,1] */
} MotionControllerConfig;

typedef struct {
    float left;
    float right;
} MotionCommand;

/* steering is normalized [-1,1]; speed is nonnegative m/s. Positive
 * steering slows the right wheel and turns the robot to the right. */
RobotStatus MotionController_Calculate(const MotionControllerConfig *config,
                                       float target_speed_mps, float steering,
                                       MotionCommand *command);
RobotStatus MotionController_Apply(const MotionCommand *command);
RobotStatus MotionController_Stop(void);

#endif
