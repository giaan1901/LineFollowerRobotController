#ifndef LINE_SENSOR_H
#define LINE_SENSOR_H

#include "robot_config.h"
#include "robot_status.h"

typedef struct {
    float strength[ROBOT_SENSOR_COUNT]; /* processed line response, [0,1] */
} LineSensorReading;

/* I2C implementation and address map are pending. */
RobotStatus LineSensor_Init(void);
RobotStatus LineSensor_Read(LineSensorReading *reading);

#endif
