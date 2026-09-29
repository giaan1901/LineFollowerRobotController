#include "motor.h"

RobotStatus Motor_Init(void)
{
    return ROBOT_NOT_IMPLEMENTED; /* TODO: configure verified driver and safe outputs. */
}

RobotStatus Motor_SetSpeed(MotorSide side, float command)
{
    if ((side != MOTOR_LEFT && side != MOTOR_RIGHT) ||
        !(command >= -1.0f && command <= 1.0f)) return ROBOT_INVALID_ARGUMENT;
    return ROBOT_NOT_IMPLEMENTED; /* TODO: drive confirmed hardware. */
}

RobotStatus Motor_Stop(void)
{
    return ROBOT_NOT_IMPLEMENTED; /* TODO: command both verified outputs safe. */
}
