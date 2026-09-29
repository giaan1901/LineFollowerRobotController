#ifndef MOTOR_H
#define MOTOR_H

#include "robot_status.h"

typedef enum { MOTOR_LEFT = 0, MOTOR_RIGHT = 1 } MotorSide;

RobotStatus Motor_Init(void);
/* Signed command in [-1,1]; sign is direction, magnitude is demand. */
RobotStatus Motor_SetSpeed(MotorSide side, float command);
RobotStatus Motor_Stop(void);

#endif
