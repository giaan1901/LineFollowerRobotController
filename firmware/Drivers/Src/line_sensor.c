#include "line_sensor.h"

RobotStatus LineSensor_Init(void)
{
    return ROBOT_NOT_IMPLEMENTED; /* TODO: configure the verified I2C devices. */
}

RobotStatus LineSensor_Read(LineSensorReading *reading)
{
    if (reading == 0) return ROBOT_INVALID_ARGUMENT;
    return ROBOT_NOT_IMPLEMENTED; /* TODO: read and normalize all five sensors. */
}
