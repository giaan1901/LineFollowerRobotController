# ME4071 Line-Following Load Transport Robot — Controller Firmware

This repository is for the robot's STM32 controller firmware only. The controller follows a marked path, monitors a carried load, and drives the transport motors.

## Hardware and requirements

- **Controller:** STM32F411CEU6.
- **Line tracking:** five line sensors provide the path position used for steering.
- **Load measurement:** a load cell read through an HX711 interface.
- **Motion:** motor drivers receive speed and direction commands from the controller.
- **Firmware behavior:** sample and validate sensor readings, steer to stay on the line, monitor load, and stop the motors when the route ends or a fault is detected.

Pin assignments, motor driver model, sensor signal levels, load limit, route-end detection, and control tuning must be confirmed against the assembled robot before implementation.

## Intended operating sequence

1. Initialize the controller, sensors, HX711, and motor outputs with the motors stopped.
2. Check sensor health and establish the load measurement baseline.
3. Once operation is enabled, read the five line sensors and drive the motors to follow the path.
4. Continue monitoring load and sensor validity during transport; stop on a fault or at the designated route end.

The start input, route-end signal, and load handling policy are to be defined with the robot's operating procedure.

## Firmware architecture

The intended modules are hardware initialization, line sensing, HX711/load measurement, motor control, route/state control, and fault handling. Keep hardware-specific pin mapping separate from control logic when firmware is added. No STM32 framework or build system has been selected yet.

## Setup

1. Confirm the development environment and choose the STM32 toolchain/framework used by the team.
2. Record the verified pin map, motor driver interface, sensor characteristics, and load-cell calibration data.
3. Add the corresponding firmware project and build instructions for that environment.
4. Build, flash, and debug on the STM32F411CEU6 hardware; validate line tracking and load measurement before enabling transport.
