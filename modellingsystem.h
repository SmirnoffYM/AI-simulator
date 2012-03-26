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
    ModellingSystem(int** map, std::pair<int, int>);
    ~ModellingSystem();

    static bool isModellingPerformed;
private:
    std::vector<Robot*> robots;
    std::vector<EnvObject*> envObjects;
    World* world;

public:
    World* getWorld()
    {
        return world;
    }

    //number is between 0 and ROBOTS-1
    Robot* getRobot(unsigned int number)
    {
        if (number < robots.size() && number < ROBOTS)
            return robots.at(number);
        else
            return NULL;
    }

    //number is between 0 and ENV_OBJECTS-1
    EnvObject* getEnvObject(unsigned int number)
    {
        if (number < envObjects.size() && number < ENV_OBJECTS)
            return envObjects.at(number);
        else
            return NULL;
    }
};

#endif // MODELLINGSYSTEM_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
