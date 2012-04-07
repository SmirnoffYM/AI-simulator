#ifndef SERVANT_H
#define SERVANT_H

#include <QColor>
#include <QProcess>
#include <QDebug>
#include <QDir>
#include <QTextStream>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <math.h>
#include <algorithm>
#include "constants.h"
#include "robot.h"
#include "hubmodule.h"

class Servant
{
    public:
        static Servant& getInstance()
        {
            static Servant instance;
            return instance;
        }

        // Managing robot apps
        void launchApplications();
        void stopApplications();
        void addApplication(QString command);

        // Loading robot profile
        Robot * buildRobot(unsigned int number);

        // Draw some object on some scene
        void drawObject(Object *object, QGraphicsScene *scene);

        // Get local map scaling for some robot
        // Robot number must be in range 0..ROBOTS-1
        unsigned int getScaling(unsigned int robotNumber)
        {
            if (scaling.find(robotNumber) != scaling.end())
                return scaling.find(robotNumber)->second;
            else
                return 1;
        }

    private:
        Servant();
        Servant(Servant const&);
        void operator=(Servant const&);

        std::vector<std::pair<QString, QProcess *> > applications;
        std::map<unsigned int, int> scaling;
};

#endif // SERVANT_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
