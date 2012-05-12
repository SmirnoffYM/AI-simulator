#ifndef ROBOTWINDOW_H
#define ROBOTWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include "constants.h"
#include "mapgraphicsscene.h"
#include "object.h"
#include "robot.h"

namespace Ui {
class RobotWindow;
}

class RobotWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit RobotWindow(QWidget *parent = 0);
    ~RobotWindow();

private:
    Ui::RobotWindow *ui;
    MapGraphicsScene *localMapScene;
    QImage *fullMap;

    unsigned int robotId;                   //robot's number. It must be from 1 to ROBOTS
    unsigned int scaling;

    bool closePermit;

    bool isIntersectionTypeDisplayed;

public slots:
    void onRefreshMap();

private:
    void closeEvent(QCloseEvent *event);
    void refreshRobotParams();
    QString buildParametersLabel();

public:
    void setRobotId(int id);
    void setMap(QImage *map);

    void setClosePermit(bool permission)
    {
        closePermit = permission;
    }

    void setScaling(unsigned int scaling);

};

#endif // ROBOTWINDOW_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
