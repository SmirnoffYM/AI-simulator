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

    localMapScene = new QGraphicsScene();
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

    paintVisibiltyCircle(robot);

    // Draw robots
    for (int i = 0; i < ROBOTS; i++) {
        Object *robot = HubModule::modellingSystem->getRobot(i);
        if (robot != NULL)
            Servant::getInstance().drawObject(robot, localMapScene);
    }
    // Draw envObjects
    for (int i = 0; i < ENV_OBJECTS; i++) {
        Object *envObject = HubModule::modellingSystem->getEnvObject(i);
        if (envObject != NULL)
            Servant::getInstance().drawObject(envObject, localMapScene);
    }

    refreshRobotParams();
}

void RobotWindow::paintVisibiltyCircle(Robot *robot)
{
    bool flag = false;              // is first circle coordinate valid?
    bool correctFlag = false;       // used by "normal" robot for skiping first point
    double prev_x;
    double prev_y;
    std::pair<int, int> worldSize = HubModule::modellingSystem->getWorld()->getSize();

    for (int angle = 0; angle <= 361; angle++) {
        double new_x = robot->getVisibilityRadius() * sin(angle * PI / 180);
        double new_y = robot->getVisibilityRadius() * cos(angle * PI / 180);

        if (!flag) {
            if (robot->getCoords().first + new_x >= 0
                    && robot->getCoords().second - new_y >= 0
                    && robot->getCoords().first + new_x < worldSize.first * REAL_PIXEL_SIZE
                    && robot->getCoords().second - new_y < worldSize.second * REAL_PIXEL_SIZE) {
                prev_x = new_x;
                prev_y = new_y;
                flag = true;
            }
            continue;
        }

        if (robot->getCoords().first + new_x < 0)
            new_x = - robot->getCoords().first;
        if (robot->getCoords().first + new_x >= worldSize.first * REAL_PIXEL_SIZE)
            new_x = worldSize.first * REAL_PIXEL_SIZE - robot->getCoords().first;
        if (robot->getCoords().second - new_y < 0)
            new_y = robot->getCoords().second;
        if (robot->getCoords().second - new_y >= worldSize.second * REAL_PIXEL_SIZE)
            new_y = - worldSize.second * REAL_PIXEL_SIZE + robot->getCoords().second;

        if (robot->getType() == Flying)
        {
            localMapScene->addLine(
                        (robot->getCoords().first + prev_x) / REAL_PIXEL_SIZE,
                        (robot->getCoords().second - prev_y) / REAL_PIXEL_SIZE,
                        (robot->getCoords().first + new_x) / REAL_PIXEL_SIZE,
                        (robot->getCoords().second - new_y) / REAL_PIXEL_SIZE,
                        QPen(QColor(robot->getColor().red(),
                                    robot->getColor().green(),
                                    robot->getColor().blue())));

            prev_x = new_x;
            prev_y = new_y;

        } else {    // if robot type is "Normal"

            bool signX = new_x > 0;
            bool signY = new_y > 0;
            bool xFaster = fabs(new_x) > fabs(new_y);

            // if x is increasinf faster than y
            if (xFaster) {

                //find each point of the line and check is it a mount
                for (int i = 0; i < static_cast<int>(fabs(new_x)) - 1; i++) {

                    double temp_x = pow(-1, static_cast<int>(!signX)) * i;
                    double temp_y = (temp_x - new_x) * (0 - new_y) / (0 - new_x) + new_y;

                    World *world = HubModule::modellingSystem->getWorld();
                    if (world->getHeight((robot->getCoords().first + temp_x) / REAL_PIXEL_SIZE,
                                         (robot->getCoords().second - temp_y) / REAL_PIXEL_SIZE) >
                        world->getHeight(robot->getCoords().first / REAL_PIXEL_SIZE,
                                         robot->getCoords().second / REAL_PIXEL_SIZE)) {
                        new_x = temp_x;
                        new_y = temp_y;
                        break;
                    }
                }
            } else {

                for (int j = 0; j < static_cast<int>(fabs(new_y)) - 1; j++) {

                    double temp_y = pow(-1, static_cast<int>(!signY)) * j;
                    double temp_x = (temp_y - new_y) * (0 - new_x) / (0 - new_y) + new_x;

                    World *world = HubModule::modellingSystem->getWorld();
                    if (world->getHeight((robot->getCoords().first + temp_x) / REAL_PIXEL_SIZE,
                                         (robot->getCoords().second - temp_y) / REAL_PIXEL_SIZE) >
                        world->getHeight(robot->getCoords().first / REAL_PIXEL_SIZE,
                                         robot->getCoords().second / REAL_PIXEL_SIZE)) {
                        new_x = temp_x;
                        new_y = temp_y;
                        break;
                    }
                }
            }

            if (correctFlag)
                localMapScene->addLine(
                            (robot->getCoords().first + prev_x) / REAL_PIXEL_SIZE,
                            (robot->getCoords().second - prev_y) / REAL_PIXEL_SIZE,
                            (robot->getCoords().first + new_x) / REAL_PIXEL_SIZE,
                            (robot->getCoords().second - new_y) / REAL_PIXEL_SIZE,
                            QPen(QColor(robot->getColor().red(),
                                        robot->getColor().green(),
                                        robot->getColor().blue())));
            else
                correctFlag = true;

            prev_x = new_x;
            prev_y = new_y;
        }
    }
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
