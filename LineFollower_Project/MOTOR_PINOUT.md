# Motor pin assignment

Use this assignment for firmware, CubeMX, schematic nets and PCB routing.

| Pin | Signal | Peripheral / alternate function |
| --- | --- | --- |
| PB9 | Motor 1 PWM / XY160D ENA | TIM11_CH1 / AF3 |
| PB8 | Motor 2 PWM / XY160D ENB | TIM10_CH1 / AF3 |
| PB7 | Motor 1 encoder B | TIM4_CH2 / AF2 |
| PB6 | Motor 1 encoder A | TIM4_CH1 / AF2 |
| PB5 | Motor 2 encoder B | TIM3_CH2 / AF2 |
| PB4 | Motor 2 encoder A | TIM3_CH1 / AF2 |
| PB3 | Motor 1 IN1 | Push-pull output, initially low |
| PA15 | Motor 1 IN2 | Push-pull output, initially low |
| PA12 | Motor 2 IN3 | Push-pull output, initially low |
| PA11 | Motor 2 IN4 | Push-pull output, initially low |

## Firmware and CubeMX settings

The supplied workspace has no `.ioc` or PCB files. The firmware initializes these
peripherals in `BSP/bsp_motor.c`, including the timer MSP callbacks.
For a future CubeMX project, use STM32F411CEUx and the table above:

- SYS Debug: Serial Wire. Reserve PA13/PA14, disable SWO trace on PB3.
- HSI: 16 MHz, no PLL; AHB, APB1 and APB2 dividers: 1.
- TIM10 and TIM11: PWM Generation CH1, prescaler 0, period 3199,
  up counter, PWM mode 1, high polarity, pulse 0, fast mode disabled.
  Both run at 5 kHz, with independent duty and no phase synchronization.
- TIM3 and TIM4: Encoder Mode TI1 and TI2, prescaler 0, period 65535,
  clock division 1, both channels direct input, rising polarity,
  input prescaler 1 and digital filter 3. This gives quadrature x4 counting.
  Encoder GPIOs have no internal pulls; provide external pulls if required.
- Four direction outputs: push-pull, no pull, low speed, initial low.
- Do not enable USB OTG FS or any other function on these ten pins.
- HAL TIM module must remain enabled. No timer interrupts are needed.

If CubeMX later generates timer handles or MSP callbacks, consolidate them with
the BSP initialization to avoid duplicate definitions and initialization.
Changing the clock requires recalculating both PWM timer periods.

Encoders and PWM start automatically. The application sets duty through
`BSP_Motor_SetDuty`. Duty is signed per mille, from -1000 to +1000.
Zero keeps PWM enabled with CCR1=0 to actively drive LOW and selects braking.
Read the raw counter through
`BSP_Motor_ReadEncoder`: TIM4 for motor 1 and TIM3 for motor 2.
Counters wrap at 65536. Compute signed modular differences at a fixed sampling
interval, keeping displacement below 32768 counts per interval. Accumulate those
differences for position. RPM requires the actual x4 counts per output-shaft
revolution and gearbox ratio; these cannot be inferred from GB37-520 alone.
Verify count sign and filter bandwidth on the actual encoders.

## Hardware checks before connection

The [XY160D supplier specification](https://www.handsontec.com/dataspecs/module/7A-160W%20motor%20control.pdf)
lists a 0-10 kHz PWM range, logic high 3.0-6.5 V, logic low 0-0.8 V,
input current 3-11 mA, and minimum valid pulse width 5 us. Other module
revisions may differ. Check the actual input current and STM32 output voltage
under that load; add a suitable buffer if the high threshold is not met.
At 5 kHz, a 5 us minimum pulse corresponds to 2.5% duty. Check the actual
module's minimum pulse specification before relying on very small duty values.
Power the module's logic supply according to its manual; logic-level compatibility
does not imply its logic supply can be powered at 3.3 V.
Both direction pins low select braking in this specification.
Provide hardware PWM pull-downs if disabled drive is required during reset.

GB37-520 describes a motor family with multiple encoder variants. For example,
[YFROBOT's Hall variant](https://yfrobot.com/en-gb/products/metal-gearmotor-gb37-520-12v-with-16-cpr)
differs from its photoelectric variant. Identify the actual encoder supply and
output circuit. For open-collector outputs, use external pull-ups to 3.3 V;
for push-pull outputs, verify voltage limits and use level translation as needed.
Check existing encoder-board pull-ups before adding any. Do not connect motor
supply voltage to the encoder signals or encoder supply.

The [STM32 datasheet](https://www.st.com/resource/en/datasheet/stm32f411rc.pdf)
confirms the AF2/AF3 timer mappings. Use the exact board revision's
[WeAct documentation](https://github.com/WeActStudio/WeActStudio.MiniSTM32F4x1)
to verify header orientation and physical pad order before making a footprint.
The requested ten-pin sequence must be checked against that footprint.
Check optional onboard SPI flash connections, particularly PB4 on some revisions.
Do not connect a USB data cable while PA11/PA12 drive the motors: those pins
remain physically wired to the board's USB connector. Program using SWD.
