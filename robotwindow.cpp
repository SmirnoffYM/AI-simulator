#include "robotwindow.h"
#include "ui_robotwindow.h"

RobotWindow::RobotWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RobotWindow)
{
    ui->setupUi(this);
    //TODO: remove all window buttons from window title
    //TODO: decide make RobotWindow modal or not
    //TODO: place this window on the right side of the desktop, but not over other RobotWindows
}

RobotWindow::~RobotWindow()
{
    delete ui;
}

void RobotWindow::onRefreshMap()
{
    //TODO: refresh local map
}

void RobotWindow::setRobotId(int id) {
    if (id <= 0 || id > ROBOTS)
        robotId = 0;
    else
        robotId = id;

    ui->robotIdValueLabel->setText(QString("%1").arg(robotId));
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
