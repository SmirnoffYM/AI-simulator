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

    std::pair<int, int> size;       //local map size
    double scaling;
};

#endif // ROBOTWINDOW_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
