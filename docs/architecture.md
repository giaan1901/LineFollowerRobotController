# Firmware architecture

## Dependency direction

`Core` starts the MCU and calls `App`. `App` owns mission state and invokes
`Control` for motion, plus driver interfaces for hardware readiness and load
measurement. `Control` may invoke `Drivers`; `Drivers` depend only on `Config`.
`Config` is shared by all layers. No driver may call an application function.

## Data flow

The planned timed loop reads five I2C sensors, normalizes each channel to a
line response in `[0,1]`, calculates a weighted line position in `[-1,1]`,
applies PID steering, mixes target speed and steering into two bounded motor
commands, then writes the motor driver. The normalized position is **not** a
distance in millimetres. Sensor geometry and edge calibration must be added
before evaluating the ±3 mm requirement.

HX711 readings, after tare and scale calibration, produce kg. The application
classifies only readings in the configured 1 kg or 2 kg windows. The selected
class is a route input; route marker recognition and branch decisions remain
unimplemented. Encoders are not assumed.

## State machine

```text
INIT --hardware ready--> FOLLOW_TO_LOADING --loading marker--> WAIT_FOR_LOAD
WAIT_FOR_LOAD --load present--> CLASSIFY_LOAD
CLASSIFY_LOAD --valid 1 kg or 2 kg--> DELIVER --destination--> STOPPED
any active state --fault--> ERROR
STOPPED or ERROR --reset--> INIT
```

Invalid transitions return `ROBOT_NOT_READY` and do not change state. A load
outside both windows stays unclassified, so delivery cannot start. `STOPPED`
and `ERROR` request a motor stop. Currently the motor adapter cannot confirm
this request, so hardware safety remains unresolved. Moving states deliberately
enter `ERROR` until the sensor, route, motor, and timed-loop integrations exist.
