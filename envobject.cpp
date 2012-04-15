#include "envobject.h"

unsigned int EnvObject::portNumber = 0;

EnvObject::EnvObject():
    Object()
{
    //default constructor
    movable = false;
    velocity = 0;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
