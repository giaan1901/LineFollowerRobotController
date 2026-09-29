#include "load_cell.h"

RobotStatus LoadCell_Init(void)
{
    return ROBOT_NOT_IMPLEMENTED; /* TODO: configure verified HX711 pins. */
}

RobotStatus LoadCell_ReadWeight(float *weight_kg)
{
    if (weight_kg == 0) return ROBOT_INVALID_ARGUMENT;
    return ROBOT_NOT_IMPLEMENTED; /* TODO: sample HX711 after calibration. */
}

RobotStatus LoadCell_Calibrate(float known_weight_kg)
{
    if (!(known_weight_kg > 0.0f)) return ROBOT_INVALID_ARGUMENT;
    return ROBOT_NOT_IMPLEMENTED; /* TODO: implement tare and scale procedure. */
}
