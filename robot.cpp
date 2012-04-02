#include "robot.h"

Robot::Robot():
    portNumber(0),
    size(0),
    orientation(0),
    intersection(AllowedForSameColor),
    coords(0, 0)
{
    //default constructor
    parameters = new std::pair<std::string, double>[CUSTOM_PARAMETERS_QUANTITY];
    for (int i = 0; i < CUSTOM_PARAMETERS_QUANTITY; i++) {
        char symbol = static_cast<char>(65 + i);
        parameters[i] = std::pair<std::string, double>(&symbol, 0);
    }

    color.red = 255;
    color.green = 255;
    color.blue = 255;
}


Robot::Robot(const Robot &copy)
{
    portNumber = copy.portNumber;
    color = copy.color;
    size = copy.size;
    orientation = copy.orientation;
    intersection = copy.intersection;
    coords = copy.coords;
    parameters = copy.parameters;
}


void Robot::setParameters(std::pair<std::string, double> *params)
{
    for (int i = 0; i < CUSTOM_PARAMETERS_QUANTITY; i++) {
        parameters[i] = params[i];
    }
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
