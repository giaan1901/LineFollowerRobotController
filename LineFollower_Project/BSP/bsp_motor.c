#include "bsp_motor.h"
#include "main.h"

static TIM_HandleTypeDef encoder[2];
static TIM_HandleTypeDef pwm[2];
static int8_t direction[2];
static uint8_t ready;

static void InitEncoder(TIM_HandleTypeDef *timer, TIM_TypeDef *instance)
{
  TIM_Encoder_InitTypeDef config = {0};
  timer->Instance = instance;
  timer->Init.Prescaler = 0;
  timer->Init.CounterMode = TIM_COUNTERMODE_UP;
  timer->Init.Period = 65535;
  timer->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  timer->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  config.EncoderMode = TIM_ENCODERMODE_TI12;
  config.IC1Polarity = config.IC2Polarity = TIM_ICPOLARITY_RISING;
  config.IC1Selection = config.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  config.IC1Prescaler = config.IC2Prescaler = TIM_ICPSC_DIV1;
  config.IC1Filter = config.IC2Filter = 3;
  if (HAL_TIM_Encoder_Init(timer, &config) != HAL_OK ||
      HAL_TIM_Encoder_Start(timer, TIM_CHANNEL_ALL) != HAL_OK)
    Error_Handler();
}

static void InitPWM(TIM_HandleTypeDef *timer, TIM_TypeDef *instance)
{
  TIM_OC_InitTypeDef config = {0};
  timer->Instance = instance;
  timer->Init.Prescaler = 0;
  timer->Init.CounterMode = TIM_COUNTERMODE_UP;
  timer->Init.Period = MOTOR_PWM_PERIOD;
  timer->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  timer->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  config.OCMode = TIM_OCMODE_PWM1;
  config.OCPolarity = TIM_OCPOLARITY_HIGH;
  config.OCFastMode = TIM_OCFAST_DISABLE;
  config.Pulse = 0;
  if (HAL_TIM_PWM_Init(timer) != HAL_OK ||
      HAL_TIM_PWM_ConfigChannel(timer, &config, TIM_CHANNEL_1) != HAL_OK)
    Error_Handler();
}

void BSP_Motor_Init(void)
{
  GPIO_InitTypeDef gpio = {0};
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  HAL_GPIO_WritePin(GPIOB, MOTOR1_IN1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, MOTOR1_IN2_Pin | MOTOR2_IN3_Pin |
                   MOTOR2_IN4_Pin, GPIO_PIN_RESET);
  gpio.Mode = GPIO_MODE_OUTPUT_PP;
  gpio.Pull = GPIO_NOPULL;
  gpio.Speed = GPIO_SPEED_FREQ_LOW;
  gpio.Pin = MOTOR1_IN1_Pin;
  HAL_GPIO_Init(GPIOB, &gpio);
  gpio.Pin = MOTOR1_IN2_Pin | MOTOR2_IN3_Pin | MOTOR2_IN4_Pin;
  HAL_GPIO_Init(GPIOA, &gpio);
  InitEncoder(&encoder[BSP_MOTOR_1], TIM4);
  InitEncoder(&encoder[BSP_MOTOR_2], TIM3);
  InitPWM(&pwm[BSP_MOTOR_1], TIM11);
  InitPWM(&pwm[BSP_MOTOR_2], TIM10);
  ready = 1;
  BSP_Motor_StopAll();
}

void BSP_Motor_SetDuty(BSP_MotorId motor, int16_t duty)
{
  if (!ready || (motor != BSP_MOTOR_1 && motor != BSP_MOTOR_2)) return;
  if (duty > 1000) duty = 1000;
  if (duty < -1000) duty = -1000;
  int8_t next = (duty > 0) ? 1 : ((duty < 0) ? -1 : 0);
  uint32_t magnitude = (uint32_t)((duty < 0) ? -duty : duty);
  TIM_HandleTypeDef *timer = &pwm[motor];
  if (next != direction[motor] || next == 0)
  {
    /* Disable CH1 immediately before changing pins; CCR is preloaded. */
    (void)HAL_TIM_PWM_Stop(timer, TIM_CHANNEL_1);
    if (motor == BSP_MOTOR_1)
    {
      HAL_GPIO_WritePin(MOTOR1_IN1_GPIO_Port, MOTOR1_IN1_Pin,
                       next > 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
      HAL_GPIO_WritePin(MOTOR1_IN2_GPIO_Port, MOTOR1_IN2_Pin,
                       next < 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
    else
    {
      HAL_GPIO_WritePin(MOTOR2_IN3_GPIO_Port, MOTOR2_IN3_Pin,
                       next > 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
      HAL_GPIO_WritePin(MOTOR2_IN4_GPIO_Port, MOTOR2_IN4_Pin,
                       next < 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
    __HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_1,
                          magnitude * (MOTOR_PWM_PERIOD + 1U) / 1000U);
    __HAL_TIM_SET_COUNTER(timer, 0);
    timer->Instance->EGR = TIM_EVENTSOURCE_UPDATE;
    direction[motor] = next;
    /* Keep CH1 enabled even at zero duty: PWM1 with CCR1=0 drives LOW.
     * Disabling CC1E leaves the alternate-function output undriven. */
    if (HAL_TIM_PWM_Start(timer, TIM_CHANNEL_1) != HAL_OK)
      Error_Handler();
  }
  else
  {
    __HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_1,
                          magnitude * (MOTOR_PWM_PERIOD + 1U) / 1000U);
  }
}

void BSP_Motor_StopAll(void)
{
  BSP_Motor_SetDuty(BSP_MOTOR_1, 0);
  BSP_Motor_SetDuty(BSP_MOTOR_2, 0);
}

uint16_t BSP_Motor_ReadEncoder(BSP_MotorId motor)
{
  if (!ready || (motor != BSP_MOTOR_1 && motor != BSP_MOTOR_2)) return 0;
  return (uint16_t)__HAL_TIM_GET_COUNTER(&encoder[motor]);
}

void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *timer)
{
  GPIO_InitTypeDef gpio = {0};
  __HAL_RCC_GPIOB_CLK_ENABLE();
  gpio.Mode = GPIO_MODE_AF_PP;
  gpio.Pull = GPIO_NOPULL; /* Use external 3.3 V pull-ups for open collectors. */
  gpio.Speed = GPIO_SPEED_FREQ_LOW;
  if (timer->Instance == TIM3)
  {
    __HAL_RCC_TIM3_CLK_ENABLE();
    gpio.Pin = MOTOR2_ENCODER_A_Pin | MOTOR2_ENCODER_B_Pin;
    gpio.Alternate = GPIO_AF2_TIM3;
  }
  else if (timer->Instance == TIM4)
  {
    __HAL_RCC_TIM4_CLK_ENABLE();
    gpio.Pin = MOTOR1_ENCODER_A_Pin | MOTOR1_ENCODER_B_Pin;
    gpio.Alternate = GPIO_AF2_TIM4;
  }
  else
  {
    return;
  }
  HAL_GPIO_Init(GPIOB, &gpio);
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *timer)
{
  GPIO_InitTypeDef gpio = {0};
  __HAL_RCC_GPIOB_CLK_ENABLE();
  gpio.Mode = GPIO_MODE_AF_PP;
  gpio.Pull = GPIO_NOPULL;
  gpio.Speed = GPIO_SPEED_FREQ_HIGH;
  if (timer->Instance == TIM10)
  {
    __HAL_RCC_TIM10_CLK_ENABLE();
    gpio.Pin = MOTOR2_PWM_Pin;
    gpio.Alternate = GPIO_AF3_TIM10;
  }
  else if (timer->Instance == TIM11)
  {
    __HAL_RCC_TIM11_CLK_ENABLE();
    gpio.Pin = MOTOR1_PWM_Pin;
    gpio.Alternate = GPIO_AF3_TIM11;
  }
  else
  {
    return;
  }
  HAL_GPIO_Init(GPIOB, &gpio);
}


