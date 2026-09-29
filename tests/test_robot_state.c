#include <assert.h>
#include "robot_state.h"

int main(void)
{
    RobotStateMachine machine;
    RobotLoadClass classification;
    assert(RobotState_Init(&machine) == ROBOT_OK);
    assert(RobotState_Transition(&machine, ROBOT_EVENT_CLASSIFIED_1KG) == ROBOT_NOT_READY);
    assert(machine.state == ROBOT_STATE_INIT);
    assert(RobotState_Transition(&machine, ROBOT_EVENT_HARDWARE_READY) == ROBOT_OK);
    assert(RobotState_Transition(&machine, ROBOT_EVENT_AT_LOADING) == ROBOT_OK);
    assert(RobotState_Transition(&machine, ROBOT_EVENT_LOAD_PRESENT) == ROBOT_OK);
    assert(RobotState_ClassifyLoad(1.0f, &classification) == ROBOT_OK);
    assert(classification == ROBOT_LOAD_1KG);
    assert(RobotState_ClassifyLoad(1.5f, &classification) == ROBOT_NOT_READY);
    assert(classification == ROBOT_LOAD_UNKNOWN);
    assert(machine.state == ROBOT_STATE_CLASSIFY_LOAD);
    assert(RobotState_Transition(&machine, ROBOT_EVENT_CLASSIFIED_2KG) == ROBOT_OK);
    assert(machine.state == ROBOT_STATE_DELIVER && machine.load_class == ROBOT_LOAD_2KG);
    assert(RobotState_Transition(&machine, ROBOT_EVENT_AT_DESTINATION) == ROBOT_OK);
    assert(machine.state == ROBOT_STATE_STOPPED);
    assert(RobotState_Transition(&machine, ROBOT_EVENT_HARDWARE_READY) == ROBOT_NOT_READY);
    assert(RobotState_Transition(&machine, ROBOT_EVENT_RESET) == ROBOT_OK);
    assert(RobotState_Transition(&machine, ROBOT_EVENT_FAULT) == ROBOT_OK);
    assert(machine.state == ROBOT_STATE_ERROR);
    return 0;
}
