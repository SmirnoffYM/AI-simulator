#ifndef ROBOT_H
#define ROBOT_H

#include <QColor>
#include "constants.h"

class Robot
{
public:
    Robot();
    Robot(const Robot &copy);

private:
    unsigned int portNumber;         //also it's robot's id
    QColor color;                    //object's color
    unsigned int size;               //diameter in special pixels (1/60 of real pixel)
    double orientation;              //orientation (in degrees)
    Intersection intersection;       //type of intersection
    std::pair<int, int> coords;      //robot coordinates, first - x, second - y;
public:
    unsigned int getPortNumber()
    {
        return portNumber;
    }

    QColor getColor()
    {
        return color;
    }

    unsigned int getSize()
    {
        return size;
    }

    double getOrientation()
    {
        return orientation;
    }

    Intersection getIntersection()
    {
        return intersection;
    }

    std::pair<int, int> getCoords()
    {
        return coords;
    }

    void setSize(int size)
    {
        this->size = size;
    }

    void setOrientation(double orientation)
    {
        if (orientation >= 360 || orientation < 0)
            this->orientation = 0;
        else
            this->orientation = orientation;
    }

    void setColor(QColor color)
    {
        this->color = color;
    }

    void setCoords(int x, int y)
    {
        coords = std::pair<int, int>(x, y);
    }
};

#endif // ROBOT_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
