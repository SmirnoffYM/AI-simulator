#include "world.h"

World::World(std::vector<std::vector<Cell> > map)
{
    heightsMap = &map;
    size = std::pair<int, int>(heightsMap->size(), heightsMap->at(0).size());
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
