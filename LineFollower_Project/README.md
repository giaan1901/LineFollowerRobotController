# LineFollower_Project

STM32F411CEUx firmware for a line follower project. The current application is
the motor bench test described in [App/README.md](App/README.md).
See [MOTOR_PINOUT.md](MOTOR_PINOUT.md) for wiring and electrical requirements.

## Open on another computer

1. Install Git and STM32CubeIDE. This project was prepared with CubeIDE 1.19.0;
   the existing generated build files identify GNU Tools for STM32 13.3.rel1.
   Use the same versions initially to minimize toolchain differences.
2. Clone this repository using its GitHub URL:
   ```sh
   git clone <repository-url>
   ```
3. Start CubeIDE and choose a workspace outside the cloned project directory.
4. Choose **File > Import > General > Existing Projects into Workspace**.
   Select the `LineFollower_Project` directory inside the clone as the root, select `LineFollower_Project`, and
   finish the import. Leave **Copy projects into workspace** unchecked so edits
   stay in the Git checkout. Keep the imported project name unchanged.
5. Select the **Debug** build configuration and build the project. CubeIDE
   generates the `Debug` directory and makefiles locally. The HAL and CMSIS
   sources are already included; no separate driver checkout is needed.
6. To flash/debug, connect an ST-LINK over SWD to the STM32F411 target, install
   the appropriate probe drivers for your OS, and create a local STM32 debug
   configuration in CubeIDE. Select the generated
   `Debug/LineFollower_Project.elf`, the ST-LINK probe, and SWD.

The motor test starts after initialization. Follow the bench-test precautions
in [App/README.md](App/README.md), including lifting the wheels before powering
the motor supply.

## Repository contents

- `App/`: motor test sequence and encoder sampling.
- `BSP/`: motor GPIO, PWM, and encoder hardware interface.
- `Core/`: main loop, interrupts, startup, and HAL configuration.
- `Drivers/`: bundled STM32 HAL and CMSIS, with their upstream license files.
- `.project`, `.cproject`, `.settings/`, and linker scripts: CubeIDE project
  configuration required to import and rebuild.

Generated binaries, build directories, local debug launch configurations, and
the existing computer-specific `.vscode` settings are excluded from Git.
CubeIDE is the supported build/debug workflow for this checkout.

There is currently no CubeMX `.ioc` file in this project. You can edit and build
the supplied firmware, but the original CubeMX configuration cannot be reopened
or regenerated from this checkout. If you have the original `.ioc`, add it to
the repository to preserve that configuration too.

## Save and sync changes

From the project directory:

```sh
git add .
git commit -m "Describe your changes"
git push
```

On another computer, commit or stash local changes before running `git pull`.
Refresh the CubeIDE project and rebuild after pulling firmware changes.

