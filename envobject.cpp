#include "envobject.h"

unsigned int EnvObject::portNumber = 8512;

EnvObject::EnvObject():
    objectId(0),
    size(0),
    orientation(0),
    intersection(AllowedForSameColor),
    movable(false),
    coords(0, 0)
{
    //default constructor
    color.red = 255;
    color.green = 255;
    color.blue = 255;
}

EnvObject::EnvObject(const EnvObject &copy)
{
    objectId = copy.objectId;
    portNumber = copy.portNumber;
    color = copy.color;
    size = copy.size;
    orientation = copy.orientation;
    intersection = copy.intersection;
    movable = copy.movable;
    coords = copy.coords;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
