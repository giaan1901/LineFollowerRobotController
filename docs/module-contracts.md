# Module contracts and ownership

All public functions return `RobotStatus` unless their declared return type is
a state or class. `ROBOT_OK` means a completed software operation;
`ROBOT_NOT_IMPLEMENTED` means a required hardware adapter is absent;
`ROBOT_NOT_READY` means valid inputs but unmet state or calibration conditions;
`ROBOT_NO_LINE` means insufficient sensor response. Invalid pointers, NaN or
out-of-range inputs return `ROBOT_INVALID_ARGUMENT` where applicable. Outputs
are not valid on an error unless explicitly documented.

| Owner | Public contract | Units and limits |
| --- | --- | --- |
| `Drivers/line_sensor` | `LineSensor_Init`, `LineSensor_Read` | Five processed strengths, each `[0,1]`; I2C details private |
| `Drivers/load_cell` | `LoadCell_Init`, `LoadCell_ReadWeight`, `LoadCell_Calibrate` | kg; positive known calibration mass |
| `Drivers/motor` | `Motor_Init`, `Motor_SetSpeed`, `Motor_Stop` | Signed demand `[-1,1]`; hardware stop pending |
| `Control/line_controller` | `Init`, `Reset`, `CalculateError`, `Update` | Normalized line error `[-1,1]`, `dt_s > 0`, bounded steering |
| `Control/motion_controller` | `Calculate`, `Apply`, `Stop` | target m/s ≥ 0, steering `[-1,1]`, measured full-command speed > 0, bounded demand |
| `App/robot_state` | `Init`, `Transition`, `ClassifyLoad` | Explicit events; kg classification windows from Config |
| `App/robot_app` | `Init`, `Update`, `GetState`, `GetLoadClass` | Validated external events only; no autonomous movement yet |
| `Core` | `main` | Add MCU initialization and periodic loop after toolchain selection |

The line controller's weighted error uses fixed logical sensor indices; it
does not meet a physical millimetre tolerance without geometry calibration.
The motion controller uses a measured m/s-at-full-command value and returns
`ROBOT_NOT_READY` when it is unset. The load windows are provisional team
choices and require calibration against the actual packages.

Module owners should change implementation files independently, keep public
headers stable through review, and add host tests for pure logic. Hardware HAL
calls belong only in driver implementations or designated adapters. The
application alone chooses load class and route; drivers never choose routes.
