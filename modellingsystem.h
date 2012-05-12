#ifndef MODELLINGSYSTEM_H
#define MODELLINGSYSTEM_H

#include <vector>
#include "constants.h"
#include "robot.h"
#include "envobject.h"
#include "world.h"

class ModellingSystem
{
public:
    ModellingSystem(int **map, std::pair<int, int>);
    ~ModellingSystem();

    static ModellingState modellingState;
    static bool isModellingStateChanged;
private:
    std::vector<Robot *> robots;
    std::vector<EnvObject *> envObjects;
    World *world;

public:
    World * getWorld()
    {
        return world;
    }

    //number is between 0 and ROBOTS-1
    Robot * getRobot(unsigned int number)
    {
        if (number < robots.size() && number < ROBOTS)
            return robots.at(number);
        else
            return NULL;
    }

    Robot * getRobotByPort(unsigned int number)
    {
        for (int i = 0; i < ROBOTS; i++)
            if (robots.at(i)->getPortNumber() == number)
                return robots.at(i);

        return NULL;
    }

    //number is between 0 and ENV_OBJECTS-1
    EnvObject * getEnvObject(unsigned int number)
    {
        if (number < envObjects.size() && number < ENV_OBJECTS)
            return envObjects.at(number);
        else
            return NULL;
    }

    int getEnvObjectPortNumber()
    {
        if (envObjects.size() > 0)
            return envObjects.at(0)->getPortNumber();
        else
            return -1; // error value
    }

    // return serial number by port number
    int getSerialByPortNumber(unsigned int number)
    {
        for  (int i = 0; i < ROBOTS; i++)
            if (robots.at(i)->getPortNumber() == number)
                return i;
        return -1; // error value
    }

    // return port number by serial
    int getPortBySerialNumber(unsigned int number)
    {
        if (number < robots.size() && number < ROBOTS)
            return robots.at(number)->getPortNumber();
        else
            return -1; // error value
    }
};

#endif // MODELLINGSYSTEM_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
