#include "app_motor_test.h"
#include "bsp_motor.h"

#define FORWARD_DUTY_PERMILLE 800 /* 80 percent forward. */
#define REVERSE_DUTY_PERMILLE (-600) /* 60 percent backward. */
#define DUTY_HOLD_MS 6000U
#define ENCODER_SAMPLE_MS 100U

volatile App_MotorTestStatus app_motor_test_status;
static uint32_t state_started_ms;
static uint32_t sampled_ms;
static uint16_t previous_count[2];

static void SetMotor1(int16_t duty)
{
  BSP_Motor_SetDuty(BSP_MOTOR_1, duty);
  app_motor_test_status.motor1_duty_permille = duty;
}

void App_MotorTest_Init(uint32_t now_ms)
{
  BSP_Motor_StopAll();
  app_motor_test_status.motor1_duty_permille = 0;
  app_motor_test_status.state = APP_TEST_WAIT;
  app_motor_test_status.now_ms = now_ms;
  app_motor_test_status.update_count = 0;
  state_started_ms = sampled_ms = now_ms;
  app_motor_test_status.sample_elapsed_ms = 0;
  for (unsigned int i = 0; i < 2; ++i)
  {
    previous_count[i] = BSP_Motor_ReadEncoder((BSP_MotorId)i);
    app_motor_test_status.encoder_count[i] = previous_count[i];
    app_motor_test_status.encoder_delta[i] = 0;
  }
  SetMotor1(FORWARD_DUTY_PERMILLE);
  app_motor_test_status.state = APP_TEST_FORWARD;
}

void App_MotorTest_Update(uint32_t now_ms)
{
  app_motor_test_status.now_ms = now_ms;
  ++app_motor_test_status.update_count;
  uint32_t elapsed = now_ms - state_started_ms;
  /* Alternate directions every six seconds without a brake interval. */
  if (elapsed >= DUTY_HOLD_MS)
  {
    switch (app_motor_test_status.state)
    {
      case APP_TEST_FORWARD:
        SetMotor1(REVERSE_DUTY_PERMILLE);
        app_motor_test_status.state = APP_TEST_REVERSE;
        break;
      case APP_TEST_REVERSE:
      default:
        SetMotor1(FORWARD_DUTY_PERMILLE);
        app_motor_test_status.state = APP_TEST_FORWARD;
        break;
    }
    state_started_ms = now_ms;
  }
  if ((uint32_t)(now_ms - sampled_ms) >= ENCODER_SAMPLE_MS)
  {
    app_motor_test_status.sample_elapsed_ms = now_ms - sampled_ms;
    sampled_ms = now_ms;
    for (unsigned int i = 0; i < 2; ++i)
    {
      uint16_t count = BSP_Motor_ReadEncoder((BSP_MotorId)i);
      uint16_t difference = (uint16_t)(count - previous_count[i]);
      app_motor_test_status.encoder_count[i] = count;
      app_motor_test_status.encoder_delta[i] = difference <= 32767U ?
          (int32_t)difference : (int32_t)difference - 65536;
      previous_count[i] = count;
    }
  }
}
