#ifndef ENVOBJECT_H
#define ENVOBJECT_H

#include <algorithm>
#include "constants.h"

class EnvObject
{
public:
    EnvObject();
    EnvObject(const EnvObject &copy);

private:
    unsigned int objectId;           //as one controller controls all envObjects,
                                     //each of them must has own id
    static unsigned int portNumber;  //controller's port number
    Color color;                    //object's color
    unsigned int size;               //diameter in special pixels (1/60 of real pixel)
    double orientation;              //orientation (in degrees)
    Intersection intersection;       //type of intersection
    bool movable;                    //is object movable
    std::pair<int, int> coords;      //object's coordinates, first - x, second - y;

public:
    // getters

    unsigned int getObjectId()
    {
        return objectId;
    }

    unsigned int getPortNumber()
    {
        return portNumber;
    }

    Color getColor()
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

    bool isMovable()
    {
        return movable;
    }

    // setters

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

    void setColor(Color color)
    {
        this->color = color;
    }

    void setCoords(int x, int y)
    {
        coords = std::pair<int, int>(x, y);
    }

    void setMovable(bool movable)
    {
        this->movable = movable;
    }
};

#endif // ENVOBJECT_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
