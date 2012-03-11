#include "robotwindow.h"
#include "ui_robotwindow.h"
#include "hubmodule.h"
#include <QCloseEvent>

RobotWindow::RobotWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RobotWindow),
    closePermit(false)
{
    ui->setupUi(this);

    robotColorScene = new QGraphicsScene();
    ui->colorGraphicsView->setScene(robotColorScene);
    localMapScene = new QGraphicsScene();
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );
    ui->robotGraphicsView->setScene(localMapScene);
    ui->colorGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->colorGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //TODO: always on top switcher
    //TODO: try to arrange RobotWindow so that they will not overlap MainWindow and other RobotWindows
}

RobotWindow::~RobotWindow()
{
    delete ui;
}

void RobotWindow::onRefreshMap()
{
    //TODO: refresh local map

    refreshRobotParams();
}

void RobotWindow::setRobotId(int id) {
    if (id <= 0 || id > ROBOTS)
        robotId = 0;
    else
        robotId = id;

    ui->robotIdValueLabel->setText(QString("%1").arg(robotId));
}

void RobotWindow::refreshRobotParams()
{
    Robot *robot = new Robot(HubModule::modellingSystem->getRobot(robotId-1));

    ui->portValueLabel->setText(QString("%1").arg(robot->getPortNumber()));
    ui->coordsValueLabel->setText(QString("%1, %2").
                                  arg(robot->getCoords().first).arg(robot->getCoords().second));
    ui->orientationValueLabel->setText(QString("%1").arg(robot->getOrientation(), 2, 'f', 1));
    ui->sizeValueLabel->setText(QString("%1").arg(robot->getSize()));

    robotColorScene->clear();
    robotColorScene->addRect(-10, -10, 20, 20, QPen(robot->getColor()), QBrush(robot->getColor()));

    QString intersectionText = QString();
    switch(robot->getIntersection()) {
    case Allowed:
        intersectionText = tr("Allowed");
        break;
    case AllowedForSameColor:
        intersectionText = tr("Allowed for same color");
        break;
    case Denied:
        intersectionText = tr("Denied");
        break;
    }
    ui->intersectionTypeValueLabel->setText(intersectionText);

    delete robot;
}

void RobotWindow::closeEvent(QCloseEvent *event)
{
    if (closePermit)
        event->accept();
    else
        event->ignore();
}

void RobotWindow::setClosePermit(bool permission)
{
    closePermit = permission;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
