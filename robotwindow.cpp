#include "robotwindow.h"
#include "ui_robotwindow.h"

RobotWindow::RobotWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RobotWindow)
{
    ui->setupUi(this);
    ui->robotIdValueLabel->setText(robotId);
    //TODO: remove all window buttons from window title
}

RobotWindow::~RobotWindow()
{
    delete ui;
}

void RobotWindow::onRefreshMap()
{
    //TODO: refresh local map
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
