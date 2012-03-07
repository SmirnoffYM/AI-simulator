#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
    In this file we'll define all constants, like
    number of robots, number of environment objects, etc.
*/

#define ROBOTS                      3           //number of robots
#define ENV_OBJECTS                 255         //number of environment objects
#define SCREEN_REFRESH_RATE         30          //screen refresh rate in times per second
#define REAL_PIXEL_SIZE             60          //number of cells in real pixel
#define MIN_MAP_SIDE                24          //minimum length of the map's side
//TODO: define MAX_MAP_SIDE

struct Cell
{
    int x;
    int y;
    int height;
};

enum Intersection { Allowed, Denied, AllowedForSameColor };

#endif // CONSTANTS_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
