#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
    In this file we'll define all constants, like
    number of robots, number of environment objects, etc.
*/

#define ROBOTS                      3           //number of robots
#define ENV_OBJECTS                 255         //number of environment objects
#define SCREEN_REFRESH_RATE         30          //screen refresh rate in seconds
#define REAL_PIXEL_SIZE             60          //number of "small pixels" in real pixel

struct Cell
{
    int x;
    int y;
    int height;
};

enum Intersection {Allowed, Denied, AllowedForSameColor};

#endif // CONSTANTS_H
