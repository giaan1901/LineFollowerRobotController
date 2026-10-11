#ifndef BSP_MOTOR_H
#define BSP_MOTOR_H

#include <stdint.h>

typedef enum { BSP_MOTOR_1 = 0, BSP_MOTOR_2 = 1 } BSP_MotorId;

void BSP_Motor_Init(void);
/* Signed duty in per mille: -1000 reverse, 0 brake, +1000 forward.
 * Stop and allow the motor to settle before reversing direction. */
void BSP_Motor_SetDuty(BSP_MotorId motor, int16_t duty);
void BSP_Motor_StopAll(void);
uint16_t BSP_Motor_ReadEncoder(BSP_MotorId motor);

#endif
