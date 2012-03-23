#include "robotwindow.h"
#include "ui_robotwindow.h"
#include "hubmodule.h"
#include <QCloseEvent>
#include <math.h>

RobotWindow::RobotWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RobotWindow),
    closePermit(false)
{
    ui->setupUi(this);

    localMapScene = new QGraphicsScene();
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );
    ui->robotGraphicsView->setScene(localMapScene);
    ui->robotGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->robotGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    isIntersectionTypeDisplayed = false;
}

RobotWindow::~RobotWindow()
{
    delete ui;
}

void RobotWindow::onRefreshMap()
{
    // Draw map
    Robot *robot = new Robot(HubModule::modellingSystem->getRobot(robotId-1));
    localMapScene->clear();

    // get top left and bottom right corners of the local map
    int x1 = robot->getCoords().first - ui->robotGraphicsView->geometry().size().width() / 2;
    int y1 = robot->getCoords().second - ui->robotGraphicsView->geometry().size().height() / 2;
    int x2 = robot->getCoords().first + ui->robotGraphicsView->geometry().size().width() / 2;
    int y2 = robot->getCoords().second + ui->robotGraphicsView->geometry().size().height() / 2;

    //FIXME: frakin map is not centered correctly
    for (int i = x1 / REAL_PIXEL_SIZE; i <= x2 / REAL_PIXEL_SIZE; i++) {
        for (int j = y1 / REAL_PIXEL_SIZE; j <= y2 / REAL_PIXEL_SIZE; j++) {

            std::pair<int, int> mapSize = HubModule::modellingSystem->getWorld()->getSize();
            int heightOfMap = 255;
            if (i >= 0 && j >= 0 && i < mapSize.first && j < mapSize.second)
                heightOfMap = HubModule::modellingSystem->getWorld()->getHeight(i, j);
            QColor pixelColor = QColor(heightOfMap, heightOfMap, heightOfMap);

            // We must "calibrate" local map to robot's position,
            // so just truncate some map pieces from left and top sides.
            // As the result robot will be on the map's center

            int width = REAL_PIXEL_SIZE;
            if ((i * REAL_PIXEL_SIZE < x1 && i >= 0) || (i * REAL_PIXEL_SIZE > x1 && i < 0))
                width -= x1 - i * REAL_PIXEL_SIZE;

            int height = REAL_PIXEL_SIZE;
            if ((j * REAL_PIXEL_SIZE < y1 && j >= 0) || (j * REAL_PIXEL_SIZE > y1 && j < 0))
                height -= y1 - j * REAL_PIXEL_SIZE;

            localMapScene->addRect((i+1) * REAL_PIXEL_SIZE - width,
                                   (j+1) * REAL_PIXEL_SIZE - height,
                                   width, height, QPen(pixelColor), QBrush(pixelColor));
        }
    }

    delete robot;

    // Draw robots
    // we must draw only robots inside our localmap rectangle
    for (int i = 0; i < ROBOTS; i++) {
        Robot *robot = new Robot(HubModule::modellingSystem->getRobot(i));

        if (robot->getCoords().first >= (int)robot->getSize() / 2
                && robot->getCoords().second >= (int)robot->getSize() / 2
                && robot->getCoords().first >= x1 && robot->getCoords().first <= x2
                && robot->getCoords().second >= y1 && robot->getCoords().second <= y2) {

            QColor outlineColor(255 - robot->getColor().red(),
                                255 - robot->getColor().green(),
                                255 - robot->getColor().blue());
            localMapScene->addEllipse(robot->getCoords().first - robot->getSize() / 2,
                                      robot->getCoords().second - robot->getSize() / 2,
                                      robot->getSize(), robot->getSize(),
                                      QPen(outlineColor), QBrush(robot->getColor()));

            double new_x = robot->getSize() / 2.0 * sin(robot->getOrientation() * PI / 180);
            double new_y = robot->getSize() / 2.0 * cos(robot->getOrientation() * PI / 180);
            localMapScene->addLine(robot->getCoords().first, robot->getCoords().second,
                                   robot->getCoords().first + new_x,
                                   robot->getCoords().second - new_y,
                                   QPen(outlineColor));
        }

        delete robot;
    }

    // Draw envObjects
    // we must draw only envObjects inside our localmap rectangle
    for (int i = 0; i < ENV_OBJECTS; i++) {
        EnvObject *envObject = new EnvObject(HubModule::modellingSystem->getEnvObject(i));

        if (envObject->getCoords().first >= (int)envObject->getSize() / 2
                && envObject->getCoords().second >= (int)envObject->getSize() / 2
                && envObject->getCoords().first >= x1 && envObject->getCoords().first <= x2
                && envObject->getCoords().second >= y1 && envObject->getCoords().second <= y2) {

            QColor outlineColor(255 - envObject->getColor().red(),
                                255 - envObject->getColor().green(),
                                255 - envObject->getColor().blue());
            localMapScene->addEllipse(envObject->getCoords().first - envObject->getSize() / 2,
                                      envObject->getCoords().second - envObject->getSize() / 2,
                                      envObject->getSize(), envObject->getSize(),
                                      QPen(outlineColor), QBrush(envObject->getColor()));

            // draw orientation line if object is movable
            if (envObject->isMovable()) {
                double new_x = envObject->getSize() / 2.0 *
                        sin(envObject->getOrientation() * PI / 180);
                double new_y = envObject->getSize() / 2.0 *
                        cos(envObject->getOrientation() * PI / 180);

                localMapScene->addLine(envObject->getCoords().first,
                                       envObject->getCoords().second,
                                       envObject->getCoords().first + new_x,
                                       envObject->getCoords().second - new_y,
                                       QPen(outlineColor));
            }
        }

        delete envObject;
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
