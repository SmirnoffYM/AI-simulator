#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
    Define all specific constants, like
    number of robots, number of environment objects, etc.:
*/

#define ROBOTS                      3           //number of robots
#define ENV_OBJECTS                 255         //number of environment objects
#define SCREEN_REFRESH_RATE         30          //screen refresh rate in times per second
#define REAL_PIXEL_SIZE             60          //number of cells in real pixel
#define ROBOT_WINDOWS_INDENT        20          //indent from the top of the screen
#define CUSTOM_PARAMETERS_QUANTITY  16          //number of custom robot params

/*

    For performance improving and memory consumption decreasing just save
    hightMap as array of integers with dimension like source grayscale map.
    But when sending messages to robot programs and environment control program
    we must use other coordinates:

        Grayscale map coordinates:      (X, Y)

                                        are equals to

        Real world map coordinates:     (X * REAL_PIXEL_SIZE, Y * REAL_PIXEL_SIZE)

    Plz, remember it when developing hub module and displaying objects on GraphicsView

*/

#define MIN_MAP_SIDE                24          //minimum length of the map's side
//TODO: define MAX_MAP_SIDE
enum Intersection { Allowed, Denied, AllowedForSameColor };

/* Define some non-crossplatform math constants: */

#define PI 3.14159265358979323846


#endif // CONSTANTS_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
