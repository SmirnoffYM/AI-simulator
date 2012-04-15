#ifndef ENVOBJECT_H
#define ENVOBJECT_H

#include <algorithm>
#include "constants.h"
#include "object.h"

class EnvObject : public virtual Object
{
public:
    EnvObject();

private:
    unsigned int objectId;              //as one controller controls all envObjects,
                                        //each of them must has own id
    static unsigned int portNumber;     //controller's port number
    unsigned int velocity;              //object's velocity

public:

    unsigned int getObjectId()
    {
        return objectId;
    }

    bool isMovable()
    {
        return movable;
    }

    unsigned int getPortNumber()
    {
        return portNumber;
    }

    unsigned int getVelocity()
    {
        return velocity;
    }

    void setMovable(bool movable)
    {
        this->movable = movable;
    }

    void setObjectId(unsigned int id)
    {
        objectId = id;
    }

    void setVelocity(unsigned int velocity)
    {
        this->velocity = velocity;
    }

    static void setPortNumber(unsigned int port)
    {
        portNumber = port;
    }
};

#endif // ENVOBJECT_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
