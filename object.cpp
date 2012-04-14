#include "object.h"

Object::Object():
    color(255, 255, 255),
    size(0),
    orientation(0),
    intersection(AllowedForSameColor),
    coords(0, 0)
{
}
