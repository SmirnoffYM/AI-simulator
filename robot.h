#ifndef ROBOT_H
#define ROBOT_H

#include <vector>
#include <algorithm>
#include <string>
#include "constants.h"
#include "object.h"

class Robot : public virtual Object
{
public:
    Robot();
    // making destructor virtual tells compiler to call most derived class' destructor when we
    // delete pointer to Robot. That way we can be sure that all resources are freed correctly. Even
    // though we don't actually derive anything from this class, we need specifier in order to
    // ensure compiler it's okay to do deletes
    virtual ~Robot() {};

private:
    unsigned int portNumber;                    // also it's robot's id
    std::pair<std::string, double> *parameters; // custom robot parameters
    unsigned int visibilityRadius;              // in this radius robot can see other objects
    double visibilityAngle;                     // visibility angle
    RobotType type;                             // type of the robot

public:

    std::vector<Object *> iCanSee();            // objects that robot sees

    std::pair<std::string, double> * getParameters()
    {
        return parameters;
    }

    bool isMovable()
    {
        return true;
    }

    unsigned int getVisibilityRadius()
    {
        return visibilityRadius;
    }

    RobotType getType()
    {
        return type;
    }

    double getVisibilityAngle()
    {
        return visibilityAngle;
    }

    void setParameters(std::pair<std::string, double> *params)
    {
        for (int i = 0; i < CUSTOM_PARAMETERS_QUANTITY; i++) {
            parameters[i] = params[i];
        }
    }

    void setParametersByID(int id, double paramValue)
    {
        std::pair<std::string, double> params;
        params.first = parameters[id].first;
        params.second = paramValue;

        parameters[id] = params;
    }

    unsigned int getPortNumber()
    {
        return portNumber;
    }

    void setPortNumber(unsigned int port)
    {
        portNumber = port;
    }

    void setVisibilityRadius(unsigned int radius)
    {
        visibilityRadius = radius;
    }

    void setType(RobotType robotType)
    {
        type = robotType;
    }

    void setVisibilityAngle(double angle)
    {
        visibilityAngle = angle;
    }
};

#endif // ROBOT_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
