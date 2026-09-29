#ifndef ROBOT_STATE_H
#define ROBOT_STATE_H

#include "robot_status.h"

typedef enum {
    ROBOT_STATE_INIT = 0,
    ROBOT_STATE_FOLLOW_TO_LOADING,
    ROBOT_STATE_WAIT_FOR_LOAD,
    ROBOT_STATE_CLASSIFY_LOAD,
    ROBOT_STATE_DELIVER,
    ROBOT_STATE_STOPPED,
    ROBOT_STATE_ERROR
} RobotStateId;

typedef enum {
    ROBOT_EVENT_HARDWARE_READY = 0,
    ROBOT_EVENT_AT_LOADING,
    ROBOT_EVENT_LOAD_PRESENT,
    ROBOT_EVENT_CLASSIFIED_1KG,
    ROBOT_EVENT_CLASSIFIED_2KG,
    ROBOT_EVENT_AT_DESTINATION,
    ROBOT_EVENT_FAULT,
    ROBOT_EVENT_RESET
} RobotEvent;

typedef enum {
    ROBOT_LOAD_UNKNOWN = 0,
    ROBOT_LOAD_1KG,
    ROBOT_LOAD_2KG
} RobotLoadClass;

typedef struct {
    RobotStateId state;
    RobotLoadClass load_class; /* route selection input, never motor control */
} RobotStateMachine;

RobotStatus RobotState_Init(RobotStateMachine *machine);
RobotStatus RobotState_Transition(RobotStateMachine *machine, RobotEvent event);
/* Returns NOT_READY outside configured classification windows. */
RobotStatus RobotState_ClassifyLoad(float weight_kg, RobotLoadClass *load_class);

#endif
