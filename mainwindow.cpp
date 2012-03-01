#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for (int i = 0; i < ROBOTS; i++)
    {
        RobotWindow* robotWindow = new RobotWindow;
        robotWindows.push_back(robotWindow);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_closePushButton_clicked()
{
    this->close();
}
