#ifndef OBJECT_H
#define OBJECT_H

#include <algorithm>
#include "constants.h"
#include "color.h"

class Object
{
public:
    Object();
    // making destructor virtual tells compiler to call most derived class' destructor when we
    // delete pointer to Object. That way we can be sure that all resources are freed correctly.
    // Even though we don't actually derive anything from this class, we need specifier in order to
    // ensure compiler it's okay to do deletes
    virtual ~Object() {};

protected:
    Color color;                     //object's color
    unsigned int size;               //diameter in special pixels (1/60 of real pixel)
    double orientation;              //orientation (in degrees)
    Intersection intersection;       //type of intersection
    bool movable;                    //is object movable
    std::pair<int, int> coords;      //object's coordinates, first - x, second - y;

public:
    // Getters

    virtual bool isMovable() = 0;

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

    // Setters

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

    void setIntersection(Intersection intersection)
    {
        this->intersection = intersection;
    }

};

#endif // OBJECT_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
