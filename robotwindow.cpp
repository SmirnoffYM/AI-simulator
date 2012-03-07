#include "robotwindow.h"
#include "ui_robotwindow.h"

RobotWindow::RobotWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RobotWindow)
{
    ui->setupUi(this);
}

RobotWindow::~RobotWindow()
{
    delete ui;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
