#include "modellingsystem.h"
#include "hubmodule.h"
#include "servant.h"

bool ModellingSystem::isModellingPerformed = false;

ModellingSystem::ModellingSystem(int **map, std::pair<int, int> size)
{
    world = new World(map, size);

    for (int i = 0; i < ROBOTS; i++) {
        Robot *robot = Servant::getInstance().buildRobot(i);
        robots.push_back(robot);
    }

    envObjects = Servant::getInstance().buildEnvironment(size);

    isModellingPerformed = false;
}

ModellingSystem::~ModellingSystem()
{
    delete world;

    for (unsigned int i = 0; i < robots.size(); i++)
        delete robots.at(i);

    for (unsigned int i = 0; i < envObjects.size(); i++)
        delete envObjects.at(i);
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
