#ifndef ROBOTWINDOW_H
#define ROBOTWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include "constants.h"

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
    QGraphicsScene* localMapScene;

    unsigned int robotId;                   //robot's number. It must be from 1 to ROBOTS
    std::pair<int, int> size;               //local map size
    double scaling;

    bool closePermit;

    bool isIntersectionTypeDisplayed;

public slots:
    void onRefreshMap();

public:
    void setRobotId(int id);
    void setClosePermit(bool permission);

private:
    void closeEvent(QCloseEvent *event);
    void refreshRobotParams();
    QString buildParametersLabel();
};

#endif // ROBOTWINDOW_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
