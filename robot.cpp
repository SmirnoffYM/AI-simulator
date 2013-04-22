#include "robot.h"
#include "envobject.h"
#include "hubmodule.h"
#include "math.h"

Robot::Robot():
    Object()
{
    //default constructor
    parameters = new std::pair<std::string, double>[CUSTOM_PARAMETERS_QUANTITY];
    for (int i = 0; i < CUSTOM_PARAMETERS_QUANTITY; i++) {
        char symbol = static_cast<char>(65 + i);
        parameters[i] = std::pair<std::string, double>(&symbol, 0);
    }
    movable = true;
    visibilityRadius = 0;
    visibilityAngle = 0;
    type = Normal;
}

std::vector<Object *> Robot::iCanSee()
{
    std::vector<Object *> objects = std::vector<Object *>();

    for (int i = 0; i < ENV_OBJECTS + ROBOTS; i++) {
        Object *object;
        if (i < ENV_OBJECTS)
            object = HubModule::modellingSystem->getEnvObject(i);
        else
            object = HubModule::modellingSystem->getRobot(i - ENV_OBJECTS);

        if (object != NULL && object->getSize() > 0 && object->getSize() / 2 + visibilityRadius
                >= sqrt(pow(object->getCoords().first - coords.first, 2)
                        + pow(object->getCoords().second - coords.second, 2))) {

            // if object == this robot, don't add it to the list
            if (i >= ENV_OBJECTS && (dynamic_cast<Robot *>(object))->getPortNumber() == portNumber)
                continue;

            // if object intersects with this robot, add it to the list
            if (object->getSize() / 2 + size / 2
                    >= sqrt(pow(object->getCoords().first - coords.first, 2)
                          + pow(object->getCoords().second - coords.second, 2))) {
                objects.push_back(object);
                continue;
            }

            // check if object is in visibilty sector
            double angle = acos(fabs(object->getCoords().second - coords.second)
                                / sqrt(pow(object->getCoords().first - coords.first, 2)
                                       + pow(object->getCoords().second - coords.second, 2)));
            angle *= 180 / PI;

            if (object->getCoords().second > coords.second
                    && object->getCoords().first > coords.first)
                angle = 180 - angle;
            else if (object->getCoords().second > coords.second
                     && object->getCoords().first <= coords.first)
                angle = 180 + angle;
            else if (object->getCoords().second <= coords.second
                     && object->getCoords().first <= coords.first)
                angle = 360 - angle;

            // check if angle between object and robot is in the needed bounds
            if (!(orientation - visibilityAngle / 2 < 0
                  && angle >= 360 + (orientation - visibilityAngle / 2))
                    &&
                 !(orientation - visibilityAngle / 2 < 0
                  && angle <= orientation + visibilityAngle / 2)
                    &&
                 !(orientation - visibilityAngle / 2 >= 0
                  && angle <= orientation + visibilityAngle / 2
                  && angle >= orientation - visibilityAngle / 2)
                    &&
                 !(orientation + visibilityAngle / 2 >= 360
                  && angle <= (orientation + visibilityAngle / 2) - 360)
                    &&
                 !(orientation + visibilityAngle / 2 >= 360
                  && angle >= orientation - visibilityAngle / 2))
                continue;

            // for "normal" robot perform check
            // for presense of a "mountain" between it and the object
            if (type == Normal) {

                double new_x = object->getCoords().first - coords.first;
                double new_y = - object->getCoords().second + coords.second;
                bool isObjectVisible = true;

                bool signX = new_x > 0;
                bool signY = new_y > 0;
                bool xFaster = fabs(new_x) > fabs(new_y);

                //FIXME: simplify this "if-else" construction

                // if x is increasing faster than y
                if (xFaster) {

                    //find each point of the line and check is it a mount
                    for (int i = 0; i < static_cast<int>(fabs(new_x)) - 1; i++) {

                        double temp_x = pow(-1, static_cast<int>(!signX)) * i;
                        double temp_y = (temp_x - new_x) * (0 - new_y) / (0 - new_x) + new_y;

                        if (sqrt(pow(temp_x, 2) + pow(temp_y, 2))
                                >= sqrt(pow(new_x, 2) + pow(new_y, 2)) - object->getSize() / 2)
                            break;

                        World *world = HubModule::modellingSystem->getWorld();
                        if (world->getHeight((coords.first + temp_x) / REAL_PIXEL_SIZE,
                                             (coords.second - temp_y) / REAL_PIXEL_SIZE) >
                                world->getHeight(coords.first / REAL_PIXEL_SIZE,
                                                 coords.second / REAL_PIXEL_SIZE)) {
                            isObjectVisible = false;
                            break;
                        }

                        //TODO: decide to check for normal robots and all envObjects on ray's way
                    }
                } else {

                    for (int j = 0; j < static_cast<int>(fabs(new_y)) - 1; j++) {

                        double temp_y = pow(-1, static_cast<int>(!signY)) * j;
                        double temp_x = (temp_y - new_y) * (0 - new_x) / (0 - new_y) + new_x;

                        if (sqrt(pow(temp_x, 2) + pow(temp_y, 2))
                                >= sqrt(pow(new_x, 2) + pow(new_y, 2)) - object->getSize() / 2)
                            break;

                        World *world = HubModule::modellingSystem->getWorld();
                        if (world->getHeight((coords.first + temp_x) / REAL_PIXEL_SIZE,
                                             (coords.second - temp_y) / REAL_PIXEL_SIZE) >
                                world->getHeight(coords.first / REAL_PIXEL_SIZE,
                                                 coords.second / REAL_PIXEL_SIZE)) {
                            isObjectVisible = false;
                            break;
                        }

                        //TODO: decide to check for normal robots and all envObjects on ray's way
                    }
                }

                if (!isObjectVisible)
                    continue;
            }

            objects.push_back(object);
        }
    }

    return objects;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
