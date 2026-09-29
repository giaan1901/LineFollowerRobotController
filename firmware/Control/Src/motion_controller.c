#include "motion_controller.h"

static float limit(float value, float max_value)
{
    if (value > max_value) return max_value;
    if (value < -max_value) return -max_value;
    return value;
}

RobotStatus MotionController_Calculate(const MotionControllerConfig *config,
                                       float target_speed_mps, float steering,
                                       MotionCommand *command)
{
    float base;
    if (config == 0 || command == 0 ||
        !(target_speed_mps >= 0.0f) ||
        !(steering >= -1.0f && steering <= 1.0f) ||
        !(config->output_limit > 0.0f && config->output_limit <= 1.0f))
        return ROBOT_INVALID_ARGUMENT;
    if (!(config->full_command_mps > 0.0f)) return ROBOT_NOT_READY;
    base = target_speed_mps / config->full_command_mps;
    command->left = limit(base * (1.0f + steering), config->output_limit);
    command->right = limit(base * (1.0f - steering), config->output_limit);
    return ROBOT_OK;
}

RobotStatus MotionController_Apply(const MotionCommand *command)
{
    RobotStatus status;
    if (command == 0 || !(command->left >= -1.0f && command->left <= 1.0f) ||
        !(command->right >= -1.0f && command->right <= 1.0f))
        return ROBOT_INVALID_ARGUMENT;
    status = Motor_SetSpeed(MOTOR_LEFT, command->left);
    if (status != ROBOT_OK) { Motor_Stop(); return status; }
    status = Motor_SetSpeed(MOTOR_RIGHT, command->right);
    if (status != ROBOT_OK) { Motor_Stop(); return status; }
    return ROBOT_OK;
}

RobotStatus MotionController_Stop(void)
{
    return Motor_Stop();
}
