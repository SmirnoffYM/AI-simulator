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
