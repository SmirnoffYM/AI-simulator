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
#define ROBOT_PARAMETER_MAX_LENGTH  15          //maximum length of robot's custom parameter name
#define SIMULATOR_PORT              9000        // port simulator binds to
#define MIN_MAP_SIDE                24          //minimum length of the map's side
#define START_IDLE_TIME             0.0000001   //start idle time for robot
#define HUB_REFRESH_TIME            1000 / SCREEN_REFRESH_RATE  // time between hub refresh
#define ROBOT_TIMEOUT               5000        // after this time being idle robot windows
                                                // will be closed
//TODO: define MAX_MAP_SIDE
enum Intersection { Allowed, Denied, AllowedForSameColor };
enum RobotType { Normal, Flying };              // Flying robot can observe the whole map
enum ModellingState {Started, Paused, Stopped};
/* Define some non-crossplatform math constants: */

#define PI 3.14159265358979323846


#endif // CONSTANTS_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
