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

    localMapScene = new QGraphicsScene();
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );
    ui->robotGraphicsView->setScene(localMapScene);
    isIntersectionTypeDisplayed = false;
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

    setWindowTitle(tr("Robot #") + QString("%1").arg(robotId));
}

void RobotWindow::refreshRobotParams()
{
    Robot *robot = new Robot(HubModule::modellingSystem->getRobot(robotId-1));

    QString params = buildParametersLabel();
    params = params.arg(robot->getCoords().first).arg(robot->getCoords().second).
                    arg(robot->getOrientation(), 2, 'f', 1).arg(robot->getSize());

    double *parameters = robot->getParameters();
    const int letterCode = 65;
    for (int i = 0; i < CUSTOM_PARAMETERS_QUANTITY; i++)
        params = params.arg(static_cast<char>(letterCode + i)).arg(parameters[i]);

    if (!isIntersectionTypeDisplayed) {
        QString winTitle = windowTitle() + QString(" | ") + tr("Intersection type: ");
        switch(robot->getIntersection()) {
        case Allowed:
            setWindowTitle(winTitle + tr("allowed"));
            break;
        case AllowedForSameColor:
            setWindowTitle(winTitle + tr("allowed for robots with same color"));
            break;
        case Denied:
            setWindowTitle(winTitle + tr("denied"));
            break;
        }
        isIntersectionTypeDisplayed = true;
    }

    ui->parametersLabel->setText(params);

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

QString RobotWindow::buildParametersLabel()
{
    const int prevParamsCount = 4;  //number of default params displayed before custom params

    QString parametersLabel =
            QString("<html><head/><body><table border=\"0\" ") +
            QString("style=\" font-size:9pt; margin-top:0px; margin-bottom:0px; ") +
            QString("margin-left:0px; margin-right:0px;\" cellspacing=\"2\" cellpadding=\"0\">");
    parametersLabel.append("<tr><td>" + tr("Coordinates") + "</td><td>%1, %2</td></tr>");
    parametersLabel.append("<tr><td>" + tr("Orientation") + "</td><td>%3</td></tr>");
    parametersLabel.append("<tr><td>" + tr("Size") + "</td><td>%4</td></tr>");

    const int start = prevParamsCount + 1;
    const int end = start + CUSTOM_PARAMETERS_QUANTITY * 2;
    for (int i = start; i < end; i += 2) {
        parametersLabel.append(QString("<tr><td>%%1</td><td>%%2</td></tr>").arg(i).arg(i+1));
    }

    parametersLabel.append("</table></body></html>");

    return parametersLabel;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
