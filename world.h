#ifndef WORLD_H
#define WORLD_H

#include "constants.h"
#include "vector"

class World
{
public:
    World();

private:
    std::vector<std::vector<Cell> > heightsMap;
    std::pair<int, int> size;                       //map size; first - width, second - height

    //TODO: world colors
};

#endif // WORLD_H