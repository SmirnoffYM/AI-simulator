#include "world.h"

World::World(int **map, std::pair<int, int> s):
    heightsMap(map),
    size(s)
{
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
