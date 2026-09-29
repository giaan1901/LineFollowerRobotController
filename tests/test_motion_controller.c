#include <assert.h>
#include "motion_controller.h"

int main(void)
{
    MotionControllerConfig config = {0.5f, 0.8f};
    MotionCommand command;
    assert(MotionController_Calculate(&config, 0.3f, 0.5f, &command) == ROBOT_OK);
    assert(command.left == 0.8f);
    assert(command.right > 0.29f && command.right < 0.31f);
    assert(MotionController_Calculate(&config, 0.0f, 0.0f, &command) == ROBOT_OK);
    assert(command.left == 0.0f && command.right == 0.0f);
    config.full_command_mps = 0.0f;
    assert(MotionController_Calculate(&config, 0.3f, 0.0f, &command) == ROBOT_NOT_READY);
    /* Hardware stop intentionally reports missing adapter. */
    assert(MotionController_Stop() == ROBOT_NOT_IMPLEMENTED);
    return 0;
}
