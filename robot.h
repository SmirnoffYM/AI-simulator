#ifndef ROBOT_H
#define ROBOT_H

#include <algorithm>
#include <string>
#include "constants.h"
#include "object.h"

class Robot : public virtual Object
{
public:
    Robot();

private:
    unsigned int portNumber;                    //also it's robot's id
    std::pair<std::string, double> *parameters; //custom robot parameters

public:
    std::pair<std::string, double> * getParameters()
    {
        return parameters;
    }

    bool isMovable()
    {
        return true;
    }

    void setParameters(std::pair<std::string, double> *params)
    {
        for (int i = 0; i < CUSTOM_PARAMETERS_QUANTITY; i++) {
            parameters[i] = params[i];
        }
    }

    unsigned int getPortNumber()
    {
        return portNumber;
    }

    void setPortNumber(unsigned int port)
    {
        portNumber = port;
    }
};

#endif // ROBOT_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
