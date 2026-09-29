# Test and validation plan

The host tests in `tests/` cover center and edge line errors, explicit
lost-line status, steering saturation, differential command limits, zero-speed
commands, missing speed calibration, load classification windows, invalid state
transitions, and stopped/error state behavior. `tests/README.md` gives build
commands for a C11 compiler. The current tests are software-only.

Before physical motion, validate in this order:

1. Verify pin map, supply rails, sensor voltage levels, motor polarity and an
   independent safe motor-disable operation at boot and on error.
2. Calibrate five sensor channels over the 26 mm line and measure sensor pitch.
   Detect disconnected, saturated and lost-line conditions.
3. Tare and calibrate the HX711 with known masses; measure repeatability and
   verify that unknown or ambiguous weights never trigger delivery.
4. Measure motor demand versus ground speed with 0, 1 and 2 kg payloads;
   determine whether encoders are available and needed.
5. Validate route markers and branches at low speed, then test 0.1 m/s and
   0.3 m/s on straights and curves down to 500 mm radius.
6. Measure line-edge deviation against ±3 mm, destination stop error against
   ±5 mm, fault-stop behavior and recovery. Record method and results before
   claiming compliance.

The physical validation steps cannot run until the board project and hardware
adapters exist. Any new route rule or safety stop path needs corresponding host
tests and physical checks.
