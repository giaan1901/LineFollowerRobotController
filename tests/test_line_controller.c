#include <assert.h>
#include "line_controller.h"

int main(void)
{
    LineSensorReading reading = {{0, 0, 1, 0, 0}};
    LineControllerConfig config = {2.0f, 0.0f, 0.0f, 0.7f, 1.0f, 0.1f};
    LineController controller;
    float error = 9.0f, steering = 9.0f;
    assert(LineController_Init(&controller, &config) == ROBOT_OK);
    assert(LineController_CalculateError(&reading, 0.1f, &error) == ROBOT_OK);
    assert(error == 0.0f);
    reading.strength[2] = 0;
    reading.strength[4] = 1;
    assert(LineController_Update(&controller, &reading, 0.1f, &steering) == ROBOT_OK);
    assert(steering == 0.7f);
    reading.strength[4] = 0;
    assert(LineController_Update(&controller, &reading, 0.1f, &steering) == ROBOT_NO_LINE);
    assert(LineController_Reset(&controller) == ROBOT_OK);
    return 0;
}
