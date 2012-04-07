#include "object.h"

Object::Object():
    size(0),
    orientation(0),
    intersection(AllowedForSameColor),
    coords(0, 0),
    color(255, 255, 255)
{
}
