#include "modellingsystem.h"

bool ModellingSystem::isModellingPerformed = false;

ModellingSystem::ModellingSystem(int **map, std::pair<int, int> size):
    world(map, size)
{
    //TODO: load ALL envObjects and robots

    for (int i = 0; i < ROBOTS; i++) {
        //TODO: load robot parameters from /robots directory
    }

    for (int i = 0; i < ENV_OBJECTS; i++) {
        //TODO: load envObject parameters from environment setting file
    }

    isModellingPerformed = true;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
