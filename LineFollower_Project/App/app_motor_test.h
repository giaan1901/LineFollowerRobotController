#ifndef APP_MOTOR_TEST_H
#define APP_MOTOR_TEST_H

#include <stdint.h>

typedef enum {
  APP_TEST_WAIT, APP_TEST_FORWARD, APP_TEST_STOP,
  APP_TEST_REVERSE, APP_TEST_DONE
} App_MotorTestState;

typedef struct {
  App_MotorTestState state;
  uint32_t now_ms;       /* HAL tick supplied by main; should keep increasing. */
  uint32_t update_count; /* Main-loop heartbeat; should keep increasing. */
  int16_t motor1_duty_permille; /* Positive forward, negative reverse. */
  uint16_t encoder_count[2];
  int32_t encoder_delta[2]; /* Counts during the latest sample interval. */
  uint32_t sample_elapsed_ms;
} App_MotorTestStatus;

/* Watch this variable in the SWD debugger. Motor 1 is index 0. */
extern volatile App_MotorTestStatus app_motor_test_status;
void App_MotorTest_Init(uint32_t now_ms);
void App_MotorTest_Update(uint32_t now_ms);

#endif
