#include "main.h"
#include "robot_app.h"

int main(void)
{
    /* TODO: add verified clock, GPIO, I2C, HX711 and motor initialization.
     * No physical control loop is started with unimplemented drivers. */
    return RobotApp_Init() == ROBOT_OK ? 0 : 1;
}
