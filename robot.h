#ifndef ROBOT_H
#define ROBOT_H

#include "constants.h"
#include <QColor>

class Robot
{
public:
    Robot();

private:
    unsigned int portNumber;         //also it's robot's id
    QColor color;                    //object's color
    unsigned int size;               //size in special pixels (1/60 of real pixel)
    double orientation;              //orientation (in degrees)
    Intersection intersection;       //type of intersection
    std::pair<int, int> coords;      //robot coordinates, first - x, second - y;
};

#endif // ROBOT_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
