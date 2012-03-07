#ifndef ENVOBJECT_H
#define ENVOBJECT_H

#include "constants.h"
#include <QColor>

class EnvObject
{
public:
    EnvObject();

private:
    unsigned int objectId;           //as one controller controls all envObjects,
                                     //each of them must has own id

    unsigned int portNumber;         //controller's port number
    QColor color;                    //object's color
    unsigned int size;               //size in special pixels (1/60 of real pixel)
    double orientation;              //orientation (in degrees)
    Intersection intersection;       //type of intersection
    bool movable;                    //is object movable
    std::pair<int, int> coords;      //object's coordinates, first - x, second - y;
};

#endif // ENVOBJECT_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
