#include "envobject.h"

unsigned int EnvObject::portNumber = 8512;

EnvObject::EnvObject():
    objectId(0),
    color(255, 255, 255),
    size(0),
    orientation(0),
    intersection(AllowedForSameColor),
    coords(0, 0),
    movable(false)
{
    //default constructor
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
