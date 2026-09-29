#ifndef LINE_CONTROLLER_H
#define LINE_CONTROLLER_H

#include "line_sensor.h"

typedef struct {
    float kp, ki, kd;
    float steering_limit; /* (0,1] */
    float integral_limit; /* >= 0, normalized-error seconds */
    float minimum_signal; /* (0,1], sum of five strengths */
} LineControllerConfig;

typedef struct {
    LineControllerConfig config;
    float integral;
    float previous_error;
    int has_previous;
} LineController;

RobotStatus LineController_Init(LineController *controller,
                                const LineControllerConfig *config);
RobotStatus LineController_Reset(LineController *controller);
/* Error is normalized to [-1,1] from the outer sensor centers; positive
 * means the line is to the right. This is not a physical-distance estimate. */
RobotStatus LineController_CalculateError(const LineSensorReading *reading,
                                           float minimum_signal,
                                           float *error);
/* dt_s must be positive; steering is clamped to +/- steering_limit. */
RobotStatus LineController_Update(LineController *controller,
                                  const LineSensorReading *reading,
                                  float dt_s, float *steering);

#endif
