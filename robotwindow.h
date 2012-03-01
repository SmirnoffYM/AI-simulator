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

    //TODO: parameters

    //FIXME: decide to use round local map
    //(for bot it can be right, e.g. visibility area) or not
    std::pair<int, int> size;       //local map size
    double scaling;
};

#endif // ROBOTWINDOW_H
