#ifndef SERVANT_H
#define SERVANT_H

#include <QColor>
#include <QProcess>
#include <QDebug>
#include <QDir>
#include <QTextStream>
#include <QGraphicsScene>
#include <math.h>
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

        // Transform from QColor to Color
        Color colorTransform(QColor col);
        QColor colorTransform(Color col);

        // Get color name
        QString getColorName(Color col);

        // Draw some object on some scene
        void drawObject(Object *object, QGraphicsScene *scene);
    private:
        Servant();
        Servant(Servant const&);
        void operator=(Servant const&);

        std::vector<std::pair<QString, QProcess *> > applications;
};

#endif // SERVANT_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
