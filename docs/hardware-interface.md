# Hardware interfaces and open decisions

| Interface | Known | Required before implementation |
| --- | --- | --- |
| MCU | STM32F411CEU6 | Toolchain, clock and peripheral project |
| Line sensing | Five I2C sensors | Model, addresses, bus, pull-ups, voltage, orientation, spacing, normalization and health checks |
| Load sensing | 5 kg cell with HX711 | GPIO pins, data timing, tare, calibration weight and scale, noise filtering |
| Motors | Two powered wheels | Motor and driver models, PWM/direction pins, voltage/current limits, polarity, brake behavior |
| Feedback | Unknown | Confirm encoders and speed measurement; otherwise document open-loop limitations |
| Supply | MP1584EN 5 V / 3 A | Measured load and grounding plan; confirm MCU and I2C logic levels |
| Route | Loading and destinations required | Marker type, branch strategy, destination recognition and timeout policy |

No pin assignments or I2C addresses are encoded yet. `hardware_config.h`
contains uncalibrated geometry, speed and load-scale fields with zero as an
unavailable value. The driver functions in `line_sensor.c`, `load_cell.c`, and
`motor.c` explicitly return `ROBOT_NOT_IMPLEMENTED`. The motor stop function
does not assert safe electrical outputs until a real driver is supplied.

Implement a fail-safe motor-disable path and validate its power-up and fault
behavior before allowing any movement. Confirm that a failed or missing sensor
reading causes the controller to stop. Do not infer physical stopping accuracy
from host tests.
