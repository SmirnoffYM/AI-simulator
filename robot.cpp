#include "robot.h"

Robot::Robot():
    Object()
{
    //default constructor
    parameters = new std::pair<std::string, double>[CUSTOM_PARAMETERS_QUANTITY];
    for (int i = 0; i < CUSTOM_PARAMETERS_QUANTITY; i++) {
        char symbol = static_cast<char>(65 + i);
        parameters[i] = std::pair<std::string, double>(&symbol, 0);
    }
    movable = true;
    visibilityRadius = 0;
    visibilityAngle = 0;
    type = Normal;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
