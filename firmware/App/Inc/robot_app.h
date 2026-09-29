#ifndef ROBOT_APP_H
#define ROBOT_APP_H

#include "robot_state.h"

/* Attempts a safe stop and hardware initialization. Until hardware adapters
 * exist this returns NOT_IMPLEMENTED and leaves the app in ERROR. */
RobotStatus RobotApp_Init(void);
/* Events must come from validated sensors/route detection, never a timer guess. */
RobotStatus RobotApp_Update(RobotEvent event);
RobotStateId RobotApp_GetState(void);
RobotLoadClass RobotApp_GetLoadClass(void);

#endif
