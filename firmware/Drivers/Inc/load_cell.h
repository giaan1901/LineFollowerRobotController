#ifndef LOAD_CELL_H
#define LOAD_CELL_H

#include "robot_status.h"

/* Weight is kg; calibration scale is kg per raw HX711 count. */
RobotStatus LoadCell_Init(void);
RobotStatus LoadCell_ReadWeight(float *weight_kg);
RobotStatus LoadCell_Calibrate(float known_weight_kg);

#endif
