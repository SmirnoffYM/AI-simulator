#include "robotwindow.h"
#include "ui_robotwindow.h"
#include <QCloseEvent>
#include <math.h>
#include "hubmodule.h"
#include "servant.h"

RobotWindow::RobotWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RobotWindow),
    closePermit(false)
{
    ui->setupUi(this);

    localMapScene = new MapGraphicsScene();
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );
    ui->robotGraphicsView->setScene(localMapScene);
    ui->robotGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->robotGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    isIntersectionTypeDisplayed = false;
    fullMap = NULL;
    scaling = 1;
}

RobotWindow::~RobotWindow()
{
    delete ui;
}

void RobotWindow::onRefreshMap()
{
    Robot *robot = HubModule::modellingSystem->getRobot(robotId-1);
    localMapScene->clear();

    // Draw some whitespaces around the map
    // It's needed for correct centering
    QColor white = QColor(255, 255, 255);

    int leftmargin = robot->getCoords().first / REAL_PIXEL_SIZE
            - ui->robotGraphicsView->geometry().size().width() / 2;
    if (leftmargin < 0) {
        localMapScene->addRect(leftmargin, 0, abs(leftmargin), fullMap->height(),
                               QPen(white), QBrush(white));
    } else {
        leftmargin = 0;
    }

    int rightmargin = robot->getCoords().first / REAL_PIXEL_SIZE
            + ui->robotGraphicsView->geometry().size().width() / 2;
    if (rightmargin > fullMap->width()) {
        rightmargin -= - fullMap->width();
        localMapScene->addRect(fullMap->width(), 0, rightmargin, fullMap->height(),
                               QPen(white), QBrush(white));
    } else {
        rightmargin = 0;
    }

    int topmargin = robot->getCoords().second / REAL_PIXEL_SIZE
            - ui->robotGraphicsView->geometry().size().height() / 2;
    if (topmargin < 0) {
        localMapScene->addRect(leftmargin, topmargin,
                               fullMap->width() + abs(leftmargin) + rightmargin, abs(topmargin),
                               QPen(white), QBrush(white));
    }

    int bottommargin = robot->getCoords().second / REAL_PIXEL_SIZE
            + ui->robotGraphicsView->geometry().size().height() / 2;
    if (bottommargin > fullMap->height()) {
        bottommargin -= - fullMap->height();
        localMapScene->addRect(leftmargin, fullMap->height(),
                               fullMap->width() + abs(leftmargin) + rightmargin, bottommargin,
                               QPen(white), QBrush(white));
    }

    // Draw a map
    localMapScene->addPixmap(QPixmap::fromImage(*fullMap));

    // Center view on robot position
    ui->robotGraphicsView->centerOn(robot->getCoords().first / REAL_PIXEL_SIZE,
                                    robot->getCoords().second / REAL_PIXEL_SIZE);

    localMapScene->drawObject(robot);

    std::vector<Object *> visibleObjects = robot->iCanSee();
    for (unsigned int i = 0; i < visibleObjects.size(); i++) {
        if (visibleObjects.at(i) != NULL)
            localMapScene->drawObject(visibleObjects.at(i));
    }

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
    Robot *robot = HubModule::modellingSystem->getRobot(robotId-1);

    QString params = buildParametersLabel();
    params = params.arg(robot->getCoords().first).arg(robot->getCoords().second).
                    arg(robot->getOrientation(), 2, 'f', 1).arg(robot->getSize());

    std::pair<std::string, double> *parameters = robot->getParameters();
    for (int i = 0; i < CUSTOM_PARAMETERS_QUANTITY; i++)
        params = params.arg(QString(parameters[i].first.c_str()).left(ROBOT_PARAMETER_MAX_LENGTH)).
                        arg(parameters[i].second);

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
}

void RobotWindow::closeEvent(QCloseEvent *event)
{
    if (closePermit)
        event->accept();
    else
        event->ignore();
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

void RobotWindow::setMap(QImage *map)
{
    if (fullMap != NULL)
        delete fullMap;
    fullMap = new QImage(*map);
}

void RobotWindow::setScaling(unsigned int scaling)
{
    ui->robotGraphicsView->scale(1.0 / this->scaling, 1.0 / this->scaling);
    this->scaling = scaling;
    ui->robotGraphicsView->scale(scaling, scaling);
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
