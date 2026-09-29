# Host tests

These tests exercise pure calculations and state transitions. The motor test
also checks that the missing hardware stop adapter reports `ROBOT_NOT_IMPLEMENTED`.
They do not validate physical stopping or line tracking.

No test framework is required. With a C11 compiler (for example GCC), build
each test from the repository root:

```sh
gcc -std=c11 -Wall -Wextra -Werror -Ifirmware/Config -Ifirmware/Drivers/Inc -Ifirmware/Control/Inc -Ifirmware/App/Inc tests/test_line_controller.c firmware/Control/Src/line_controller.c -o test_line_controller
gcc -std=c11 -Wall -Wextra -Werror -Ifirmware/Config -Ifirmware/Drivers/Inc -Ifirmware/Control/Inc -Ifirmware/App/Inc tests/test_motion_controller.c firmware/Control/Src/motion_controller.c firmware/Drivers/Src/motor.c -o test_motion_controller
gcc -std=c11 -Wall -Wextra -Werror -Ifirmware/Config -Ifirmware/App/Inc tests/test_robot_state.c firmware/App/Src/robot_state.c -o test_robot_state
./test_line_controller
./test_motion_controller
./test_robot_state
```

On Windows, run the generated `.exe` files instead. A firmware toolchain and
board build must be added after the team selects an STM32 environment.
