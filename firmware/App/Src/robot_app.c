#include "robot_app.h"
#include "motion_controller.h"
#include "line_sensor.h"
#include "load_cell.h"
#include "motor.h"

static RobotStateMachine app_state = {ROBOT_STATE_ERROR, ROBOT_LOAD_UNKNOWN};

RobotStatus RobotApp_Init(void)
{
    RobotStatus status;
    RobotState_Init(&app_state);
    status = Motor_Init();
    if (status != ROBOT_OK) goto fail;
    status = MotionController_Stop();
    if (status != ROBOT_OK) goto fail;
    status = LineSensor_Init();
    if (status != ROBOT_OK) goto fail;
    status = LoadCell_Init();
    if (status != ROBOT_OK) goto fail;
    return ROBOT_OK;
fail:
    MotionController_Stop(); /* best effort; actual safety requires hardware */
    RobotState_Transition(&app_state, ROBOT_EVENT_FAULT);
    return status;
}

RobotStatus RobotApp_Update(RobotEvent event)
{
    RobotStatus status;
    if (app_state.state == ROBOT_STATE_ERROR ||
        app_state.state == ROBOT_STATE_STOPPED) {
        MotionController_Stop();
        return ROBOT_NOT_READY;
    }
    status = RobotState_Transition(&app_state, event);
    if (status != ROBOT_OK) return status;
    if (app_state.state == ROBOT_STATE_WAIT_FOR_LOAD ||
        app_state.state == ROBOT_STATE_CLASSIFY_LOAD ||
        app_state.state == ROBOT_STATE_STOPPED ||
        app_state.state == ROBOT_STATE_ERROR) {
        status = MotionController_Stop();
        if (status != ROBOT_OK) {
            RobotState_Transition(&app_state, ROBOT_EVENT_FAULT);
            return status;
        }
    }
    if (app_state.state == ROBOT_STATE_FOLLOW_TO_LOADING ||
        app_state.state == ROBOT_STATE_DELIVER) {
        /* TODO: route markers, sensor reads, controller timing and target speed. */
        MotionController_Stop();
        RobotState_Transition(&app_state, ROBOT_EVENT_FAULT);
        return ROBOT_NOT_IMPLEMENTED;
    }
    return ROBOT_OK;
}

RobotStateId RobotApp_GetState(void) { return app_state.state; }
RobotLoadClass RobotApp_GetLoadClass(void) { return app_state.load_class; }
