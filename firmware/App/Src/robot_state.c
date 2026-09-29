#include "robot_state.h"
#include "robot_config.h"

RobotStatus RobotState_Init(RobotStateMachine *machine)
{
    if (machine == 0) return ROBOT_INVALID_ARGUMENT;
    machine->state = ROBOT_STATE_INIT;
    machine->load_class = ROBOT_LOAD_UNKNOWN;
    return ROBOT_OK;
}

RobotStatus RobotState_ClassifyLoad(float weight_kg, RobotLoadClass *load_class)
{
    if (load_class == 0 || !(weight_kg >= 0.0f))
        return ROBOT_INVALID_ARGUMENT;
    *load_class = ROBOT_LOAD_UNKNOWN;
    if (weight_kg >= ROBOT_LOAD_1KG_MIN_KG &&
        weight_kg <= ROBOT_LOAD_1KG_MAX_KG) {
        *load_class = ROBOT_LOAD_1KG;
        return ROBOT_OK;
    }
    if (weight_kg >= ROBOT_LOAD_2KG_MIN_KG &&
        weight_kg <= ROBOT_LOAD_2KG_MAX_KG) {
        *load_class = ROBOT_LOAD_2KG;
        return ROBOT_OK;
    }
    return ROBOT_NOT_READY;
}

RobotStatus RobotState_Transition(RobotStateMachine *machine, RobotEvent event)
{
    if (machine == 0 || event < ROBOT_EVENT_HARDWARE_READY ||
        event > ROBOT_EVENT_RESET) return ROBOT_INVALID_ARGUMENT;
    if (event == ROBOT_EVENT_FAULT) {
        machine->state = ROBOT_STATE_ERROR;
        return ROBOT_OK;
    }
    if (event == ROBOT_EVENT_RESET &&
        (machine->state == ROBOT_STATE_STOPPED ||
         machine->state == ROBOT_STATE_ERROR))
        return RobotState_Init(machine);
    switch (machine->state) {
    case ROBOT_STATE_INIT:
        if (event == ROBOT_EVENT_HARDWARE_READY)
            machine->state = ROBOT_STATE_FOLLOW_TO_LOADING;
        else return ROBOT_NOT_READY;
        break;
    case ROBOT_STATE_FOLLOW_TO_LOADING:
        if (event == ROBOT_EVENT_AT_LOADING)
            machine->state = ROBOT_STATE_WAIT_FOR_LOAD;
        else return ROBOT_NOT_READY;
        break;
    case ROBOT_STATE_WAIT_FOR_LOAD:
        if (event == ROBOT_EVENT_LOAD_PRESENT)
            machine->state = ROBOT_STATE_CLASSIFY_LOAD;
        else return ROBOT_NOT_READY;
        break;
    case ROBOT_STATE_CLASSIFY_LOAD:
        if (event == ROBOT_EVENT_CLASSIFIED_1KG) {
            machine->load_class = ROBOT_LOAD_1KG;
            machine->state = ROBOT_STATE_DELIVER;
        } else if (event == ROBOT_EVENT_CLASSIFIED_2KG) {
            machine->load_class = ROBOT_LOAD_2KG;
            machine->state = ROBOT_STATE_DELIVER;
        } else return ROBOT_NOT_READY;
        break;
    case ROBOT_STATE_DELIVER:
        if (event == ROBOT_EVENT_AT_DESTINATION)
            machine->state = ROBOT_STATE_STOPPED;
        else return ROBOT_NOT_READY;
        break;
    case ROBOT_STATE_STOPPED:
    case ROBOT_STATE_ERROR:
        return ROBOT_NOT_READY;
    default:
        return ROBOT_INVALID_ARGUMENT;
    }
    return ROBOT_OK;
}
