#ifndef MODELLINGSYSTEM_H
#define MODELLINGSYSTEM_H

#include "constants.h"
#include "list"
#include "algorithm"
#include "robot.h"
#include "envobject.h"
#include "world.h"

class ModellingSystem
{
public:
    ModellingSystem();

private:
    std::list<Robot*> robots;
    std::list<EnvObject*> envObjects;
    World* world;

    bool loadMap(std::string fileName);
};

#endif // MODELLINGSYSTEM_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
