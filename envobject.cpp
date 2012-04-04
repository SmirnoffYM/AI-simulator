#include "envobject.h"

unsigned int EnvObject::portNumber = ENV_OBJECT_DEFAULT_PORT;

EnvObject::EnvObject():
    Object()
{
    //default constructor
    movable = false;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
