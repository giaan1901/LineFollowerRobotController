#include "line_controller.h"

static float clamp(float value, float limit)
{
    if (value > limit) return limit;
    if (value < -limit) return -limit;
    return value;
}

RobotStatus LineController_Init(LineController *controller,
                                const LineControllerConfig *config)
{
    if (controller == 0 || config == 0 ||
        !(config->steering_limit > 0.0f && config->steering_limit <= 1.0f) ||
        !(config->integral_limit >= 0.0f) ||
        !(config->minimum_signal > 0.0f && config->minimum_signal <= 1.0f))
        return ROBOT_INVALID_ARGUMENT;
    controller->config = *config;
    return LineController_Reset(controller);
}

RobotStatus LineController_Reset(LineController *controller)
{
    if (controller == 0) return ROBOT_INVALID_ARGUMENT;
    controller->integral = 0.0f;
    controller->previous_error = 0.0f;
    controller->has_previous = 0;
    return ROBOT_OK;
}

RobotStatus LineController_CalculateError(const LineSensorReading *reading,
                                           float minimum_signal,
                                           float *error)
{
    static const float position[ROBOT_SENSOR_COUNT] =
        {-1.0f, -0.5f, 0.0f, 0.5f, 1.0f};
    float sum = 0.0f, weighted = 0.0f;
    unsigned int i;
    if (reading == 0 || error == 0 ||
        !(minimum_signal > 0.0f && minimum_signal <= 1.0f))
        return ROBOT_INVALID_ARGUMENT;
    for (i = 0; i < ROBOT_SENSOR_COUNT; ++i) {
        float value = reading->strength[i];
        if (!(value >= 0.0f && value <= 1.0f)) return ROBOT_INVALID_ARGUMENT;
        sum += value;
        weighted += value * position[i];
    }
    if (sum < minimum_signal) return ROBOT_NO_LINE;
    *error = weighted / sum;
    return ROBOT_OK;
}

RobotStatus LineController_Update(LineController *controller,
                                  const LineSensorReading *reading,
                                  float dt_s, float *steering)
{
    float error, derivative, next_integral, output;
    RobotStatus status;
    if (controller == 0 || steering == 0 || !(dt_s > 0.0f))
        return ROBOT_INVALID_ARGUMENT;
    status = LineController_CalculateError(reading,
                controller->config.minimum_signal, &error);
    if (status != ROBOT_OK) return status;
    next_integral = clamp(controller->integral + error * dt_s,
                           controller->config.integral_limit);
    derivative = controller->has_previous ?
        (error - controller->previous_error) / dt_s : 0.0f;
    output = controller->config.kp * error +
             controller->config.ki * next_integral +
             controller->config.kd * derivative;
    *steering = clamp(output, controller->config.steering_limit);
    controller->integral = next_integral;
    controller->previous_error = error;
    controller->has_previous = 1;
    return ROBOT_OK;
}
