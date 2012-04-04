#include "object.h"

Object::Object():
    size(0),
    orientation(0),
    intersection(AllowedForSameColor),
    coords(0, 0)
{
    color.red = 255;
    color.green = 255;
    color.blue = 255;
}
