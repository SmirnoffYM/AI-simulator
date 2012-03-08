#ifndef ROBOTWINDOW_H
#define ROBOTWINDOW_H

#include <QWidget>
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

    unsigned int robotId;           //robot's number. It must be from 1 to ROBOTS
    //TODO: parameters

    std::pair<int, int> size;       //local map size
    double scaling;

public slots:
    void onRefreshMap();

public:
    void setRobotId(int id) {
        if (id <= 0 || id > ROBOTS)
            robotId = 0;
        else
            robotId = id;
    }
};

#endif // ROBOTWINDOW_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
