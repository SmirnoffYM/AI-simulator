#include "robot.h"

Robot::Robot():
    portNumber(0),
    color(255, 255, 255),
    size(0),
    orientation(0),
    intersection(AllowedForSameColor),
    coords(0, 0)
{
    //default constructor
}


Robot::Robot(const Robot &copy)
{
    portNumber = copy.portNumber;
    color = copy.color;
    size = copy.size;
    orientation = copy.orientation;
    intersection = copy.intersection;
    coords = copy.coords;
}


/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
