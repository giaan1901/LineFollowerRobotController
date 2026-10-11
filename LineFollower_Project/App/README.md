# Motor bench test

`main.c` initializes HAL, the 16 MHz clock, BSP hardware, and the application.
It calls `App_MotorTest_Update(HAL_GetTick())` continuously.

- `App/app_motor_test.c` owns the sequence, timing, and encoder sample logic.
  It depends only on the BSP interface and receives time from main.
- `BSP/bsp_motor.c` owns GPIO, PWM, encoder timers, and HAL MSP callbacks.
  Keep future ADC and other hardware access in BSP modules and call them from App.
- `Core` keeps startup, clock setup, interrupts, and the main loop.

The test repeats every 12 seconds after reset, without a brake interval:

| Time after startup | Motor 1 (motor 2 stays stopped) |
| --- | --- |
| 0-6 seconds | Forward at 80% duty |
| 6-12 seconds | Reverse at 60% duty |
| After 12 seconds | Repeat from forward |

Lift the wheels before powering the motor supply. Flash and debug over SWD;
the motor starts forward immediately after initialization.
Tune `FORWARD_DUTY_PERMILLE`, `REVERSE_DUTY_PERMILLE`, and `DUTY_HOLD_MS`
at the top of `app_motor_test.c`. Duty is open loop: it does not regulate RPM.
Direction changes have no application delay; BSP briefly disables PWM while
switching the direction pins and loading the new duty.

Watch `app_motor_test_status` in the debugger. `motor1_duty_permille` shows
the signed command (800 = 80% forward, -600 = 60% reverse).
`now_ms` and `update_count` should keep increasing while the CPU runs.
At reverse duty, PB3 should be LOW and PA15 HIGH; both are LOW at 0%.
At forward duty, PB3 should be HIGH and PA15 LOW.
TIM11 CCR1 should alternate between 2560 and 1920, with ARR=3199.
Index 0 is motor 1; index 1 is
motor 2. `encoder_count` is the raw 16-bit counter and `encoder_delta` is the
signed count change over `sample_elapsed_ms` (normally 100 ms). Deltas handle
counter wrap provided displacement is less than 32768 counts per sample.
Forward count sign depends on wiring. RPM conversion needs your encoder's
actual counts per output revolution. No UART or ADC is configured for this test.

Debug and Release CubeIDE configurations include both App and BSP sources and
headers. Refresh the project before building if the IDE was already open.
See `MOTOR_PINOUT.md` for the fixed assignment and electrical requirements.
