# ME4071 Line-Following Load Transport Robot — controller firmware

This repository holds the STM32F411CEU6 controller firmware, design contracts,
and host-side tests. It is a scaffold for team development, **not runnable robot
firmware**: the I2C sensors, HX711, motor outputs, route markers, and MCU project
have not been implemented or validated.

## Operating sequence

`INIT` checks hardware, then the robot follows the line to the loading station.
It stops, waits for a package, classifies a measured load as 1 kg or 2 kg, and
only then enters `DELIVER`. A validated destination marker leads to `STOPPED`;
any unrecoverable fault leads to `ERROR`. The route for each load class remains
to be defined. The application currently refuses to move and reports missing
hardware functionality.

## Requirements and initial hardware

| Item | Requirement or current choice |
| --- | --- |
| MCU | STM32F411CEU6 |
| Drive | Differential drive, two powered wheels and two casters |
| Track | 26 mm black line on white; minimum curve radius 500 mm |
| Speed | At least 0.1 m/s; design 0.3 m/s |
| Tolerances | ±3 mm from line edge while following; ±5 mm final stop |
| Mass | 4 kg design total, including at most 2 kg payload |
| Line sensors | Five I2C sensors; model, spacing and addresses unknown |
| Load sensing | 5 kg load cell with HX711; calibration unknown |
| Supply | MP1584EN 5 V / 3 A regulator |

Motor and driver models, encoder availability, all pin assignments, sensor
electrical details, and route markers are unresolved. The PID defaults and load
classification windows in `robot_config.h` are starting software choices, not
validated tuning or calibration. Confirm the electrical design and power budget
before using hardware.

## Repository tree

```text
.
├── README.md                     project status and workflow
├── .gitignore                    generated and local files
├── docs/
│   ├── architecture.md           data flow, dependencies, states
│   ├── hardware-interface.md     electrical interfaces and unknowns
│   ├── module-contracts.md       APIs, units and ownership
│   └── testing.md                host and physical validation plan
├── firmware/
│   ├── Core/Inc/main.h            MCU integration declaration placeholder
│   ├── Core/Src/main.c            startup integration placeholder
│   ├── App/Inc/                    robot_app.h, robot_state.h
│   ├── App/Src/                    robot_app.c, robot_state.c
│   ├── Control/Inc/                line_controller.h, motion_controller.h
│   ├── Control/Src/                line_controller.c, motion_controller.c
│   ├── Drivers/Inc/                line_sensor.h, load_cell.h, motor.h
│   ├── Drivers/Src/                line_sensor.c, load_cell.c, motor.c
│   └── Config/                     robot_config.h, hardware_config.h,
│                                  robot_status.h
└── tests/                         host tests and instructions
```

## Modules and data flow

`Core` owns STM32 startup and invokes `App`; it must preserve generated code
when a CubeMX project is added. `App` owns states, load class and route choice.
`Control` owns line position/PID calculations and differential motor mixing.
`Drivers` own hardware access only; they must never depend on `App` or
`Control`. `Config` holds shared requirements and calibration inputs.

The intended path is **I2C readings → processed five-sensor response → line
error → steering correction → left/right commands → motor driver**. HX711
weight feeds load classification in `App`; the class selects a route, not a
motor command. See [architecture](docs/architecture.md) and
[contracts](docs/module-contracts.md) for details.

Each teammate can own one module pair (`.h` and `.c`) while using its public
header as the contract. Coordinate changes to shared status and configuration
types before merging. Keep hardware code behind driver APIs and add host tests
for any new calculation or state rule.

## Build and development

No STM32 toolchain, HAL project, startup file, linker script, or board build is
present. Select the team toolchain, generate or import its MCU project, and
integrate these modules without replacing generated startup code. Record the
pin map and calibrations in the hardware documentation. Then build, flash, and
validate with motors disabled before enabling movement.

Host test commands are in [tests/README.md](tests/README.md). Tests exercise
software logic only. Development order: verify hardware assignments; implement
safe motor stop; implement sensor and load adapters; calibrate readings and
speed mapping; add route detection; integrate the timed control loop; run host
tests and physical validation. See [testing](docs/testing.md).

## Current status

Line error calculation, bounded PID correction, bounded differential command
calculation, load-window classification, and explicit state transitions are
implemented in platform-independent C. Hardware driver functions deliberately
return `ROBOT_NOT_IMPLEMENTED`; physical safe stopping is **not yet guaranteed**.
`RobotApp_Init` enters `ERROR` until the required adapters work. Open decisions
and adapter tasks are listed in [hardware-interface](docs/hardware-interface.md).
