#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "constants.h"

class World
{
public:
    World(int** map, std::pair<int, int> s);

    std::pair<int, int> getSize()
    {
        return size;
    }

    int getHeight(int x, int y)
    {
        if (x >= 0 && x < size.first && y >= 0 && y < size.second)
            return heightsMap[x][y];
        else
            return 0;
    }

    int ** getHeightsMap()
    {
        return heightsMap;
    }

private:
    int **heightsMap;
    std::pair<int, int> size;                       //map size; first - height, second - width

    //TODO: world colors
};

#endif // WORLD_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
