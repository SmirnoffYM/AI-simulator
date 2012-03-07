#include "modellingsystem.h"

ModellingSystem::ModellingSystem(std::vector<std::vector<Cell> > map)
{
    //TODO: load ALL envObjects and robots

    world = new World(map);
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
