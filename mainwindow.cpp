#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);

    const QRect screen = QApplication::desktop()->screenGeometry();

    this->showMaximized();

    for (int i = 0; i < ROBOTS; i++)
    {

        RobotWindow* robotWindow = new RobotWindow;

        int delta = robotWindow->frameGeometry().height() -
                    (screen.height() - ROBOT_WINDOWS_INDENT) / ROBOTS;

        if (delta > 0)
            delta = screen.height() - delta;
        else
            delta = screen.height();


        robotWindow->move(screen.width() - robotWindow->width(),
                          i * delta / ROBOTS - ROBOT_WINDOWS_INDENT);

        robotWindows.push_back(robotWindow);
        robotWindows.at(i)->setRobotId(i+1);
    }

    modellingPaused = false;
    mapOpened = false;
    map = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    for (int i = 0; i < ROBOTS; i++) {
        robotWindows.at(i)->setClosePermit(true);
        robotWindows.at(i)->close();
    }
}

void MainWindow::on_action_Exit_triggered()
{
    stopModelling();
    this->close();
}

void MainWindow::on_action_Open_map_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open map file"), "map",
            tr("Grayscale map files (*.*)"));

    if (fileName.isEmpty())
        return;

    QImage *image = new QImage(fileName);

    if (isMapCorrect(*image)) {

        // destroy old modelling system if it exists
        if (HubModule::modellingSystem != NULL) {
            scene->clear();
            HubModule::modellingSystem->~ModellingSystem();
        }

        // create new modelling system with map from an image
        std::pair<int, int> size = std::pair<int, int>(image->height(), image->width());
        HubModule::modellingSystem = new ModellingSystem(loadMap(*image), size);
        // Store opened map
        // We'll reload it if user will stop modelling and then start it without changing map
        if (map != NULL)
            delete map;
        map = new QImage(*image);
        drawMap(map);
        delete image;

        validateButtons(Stopped);

        mapOpened = true;
    } else {
        QMessageBox::critical(this, tr("Error!"), tr("Invalid map file!"));
        delete image;
    }
}

void MainWindow::on_actionAbout_Program_triggered()
{
    //TODO: description

    QMessageBox::information(this, tr("AI simulator"), tr("description..."));
}

void MainWindow::on_actionRun_triggered()
{
    // if modelling was stopped and then started again
    if (HubModule::modellingSystem != NULL && !HubModule::modellingSystem->isModellingPerformed
            && !modellingPaused && !mapOpened) {
        int **heightMap = HubModule::modellingSystem->getWorld()->getHeightsMap();
        std::pair<int, int> size = HubModule::modellingSystem->getWorld()->getSize();
        HubModule::modellingSystem->~ModellingSystem();
        HubModule::modellingSystem = new ModellingSystem(heightMap, size);
        drawMap(map);
    }

    // if modelling was not paused, start hub and show all robowindows
    if (!modellingPaused) {
        //TODO: start hub thread
        hub = new HubModule();

        for (int i = 0; i < ROBOTS; i++) {
            robotWindows.at(i)->setMap(map);
            robotWindows.at(i)->show();
        }
    }

    HubModule::modellingSystem->isModellingPerformed = true;
    modellingPaused = false;
    validateButtons(Started);
    QTimer::singleShot(0, this, SLOT(onRefreshMap()));
}

void MainWindow::on_actionPause_triggered()
{
    HubModule::modellingSystem->isModellingPerformed = false;
    modellingPaused = true;
    validateButtons(Paused);
}

void MainWindow::on_actionStop_triggered()
{
    HubModule::modellingSystem->isModellingPerformed = false;
    modellingPaused = false;
    mapOpened = false;
    stopModelling();
    validateButtons(Stopped);
}

void MainWindow::stopModelling()
{
    for (int i = 0; i < ROBOTS; i++) {
        robotWindows.at(i)->hide();
    }

    //TODO: close all threads
}

void MainWindow::validateButtons(ButtonsState state)
{
    switch(state) {
    case Started:
        ui->action_Open_map->setEnabled(false);
        ui->actionRun->setEnabled(false);
        ui->actionPause->setEnabled(true);
        ui->actionStop->setEnabled(true);
        break;
    case Paused:
        ui->action_Open_map->setEnabled(false);
        ui->actionRun->setEnabled(true);
        ui->actionPause->setEnabled(false);
        ui->actionStop->setEnabled(true);
        break;
    case Stopped:
        ui->action_Open_map->setEnabled(true);
        ui->actionRun->setEnabled(true);
        ui->actionPause->setEnabled(false);
        ui->actionStop->setEnabled(false);
        break;
    }
}

bool MainWindow::isMapCorrect(QImage image)
{
    if (!image.isNull() && image.isGrayscale()
            && image.height() >= MIN_MAP_SIDE && image.width() >= MIN_MAP_SIDE)
        return true;
    else
        return false;
}

int ** MainWindow::loadMap(QImage image)
{
    const int height = image.height();
    const int width = image.width();

    //I hate c++ for this:
    int** map = new int*[height];
    for (int i = 0; i < height; i++)
        map[i] = new int[width];

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            map[i][j] = qGray(image.pixel(i, j));

    return map;
}

void MainWindow::onRefreshMap()
{
    if (ModellingSystem::isModellingPerformed) {

        drawMap(map);

        // Draw all robots
        // Each robot is a circle colored with robot->getColor()
        // Robot's orientation is indicated by line with inverted circle color
        // Line links circle's center and circle's outline
        for (int i = 0; i < ROBOTS; i++) {
            Robot *robot = HubModule::modellingSystem->getRobot(i);

            if (robot->getCoords().first >= static_cast<int>(robot->getSize() / 2)
                    && robot->getCoords().second >= static_cast<int>(robot->getSize() / 2)
                    && robot->getSize() > 0
                    && robot->getCoords().first + static_cast<int>(robot->getSize() / 2) <=
                       HubModule::modellingSystem->getWorld()->getSize().first * REAL_PIXEL_SIZE
                    && robot->getCoords().second + static_cast<int>(robot->getSize() / 2) <=
                       HubModule::modellingSystem->getWorld()->getSize().second * REAL_PIXEL_SIZE) {
                QColor outlineColor(255 - robot->getColor().red(),
                                    255 - robot->getColor().green(),
                                    255 - robot->getColor().blue());

                int circle_x = (robot->getCoords().first - robot->getSize() / 2) / REAL_PIXEL_SIZE;
                int circle_y = (robot->getCoords().second - robot->getSize() / 2) / REAL_PIXEL_SIZE;

                scene->addEllipse(circle_x, circle_y,
                                  robot->getSize() / REAL_PIXEL_SIZE,
                                  robot->getSize() / REAL_PIXEL_SIZE,
                                  QPen(outlineColor),
                                  QBrush(robot->getColor()));

                double new_x = robot->getSize() / 2.0 * sin(robot->getOrientation() * PI / 180);
                double new_y = robot->getSize() / 2.0 * cos(robot->getOrientation() * PI / 180);

                scene->addLine(robot->getCoords().first / REAL_PIXEL_SIZE,
                               robot->getCoords().second / REAL_PIXEL_SIZE,
                               (robot->getCoords().first + new_x) / REAL_PIXEL_SIZE,
                               (robot->getCoords().second - new_y) / REAL_PIXEL_SIZE,
                               QPen(outlineColor));
            }
        }

        // Draw all envObjects
        // Each envObject is a circle colored with robot->getColor()
        // If envObject is movable, it has orientation
        // Orientation is indicated by line with inverted circle color
        // Line links circle's center and circle's outline
        for (int i = 0; i < ENV_OBJECTS; i++) {
            EnvObject *envObject = HubModule::modellingSystem->getEnvObject(i);

            if (envObject->getCoords().first >= static_cast<int>(envObject->getSize() / 2)
                    && envObject->getCoords().second >= static_cast<int>(envObject->getSize() / 2)
                    && envObject->getSize() > 0
                    && envObject->getCoords().first + static_cast<int>(envObject->getSize() / 2) <=
                       HubModule::modellingSystem->getWorld()->getSize().first * REAL_PIXEL_SIZE
                    && envObject->getCoords().second + static_cast<int>(envObject->getSize() / 2) <=
                       HubModule::modellingSystem->getWorld()->getSize().second * REAL_PIXEL_SIZE) {
                QColor outlineColor(255 - envObject->getColor().red(),
                                    255 - envObject->getColor().green(),
                                    255 - envObject->getColor().blue());

                int circle_x = (envObject->getCoords().first -
                                envObject->getSize() / 2) / REAL_PIXEL_SIZE;
                int circle_y = (envObject->getCoords().second -
                                envObject->getSize() / 2) / REAL_PIXEL_SIZE;

                scene->addEllipse(circle_x, circle_y,
                                  envObject->getSize() / REAL_PIXEL_SIZE,
                                  envObject->getSize() / REAL_PIXEL_SIZE,
                                  QPen(outlineColor), QBrush(envObject->getColor()));

                // draw orientation line if object is movable
                if (envObject->isMovable()) {
                    double new_x = envObject->getSize() / 2.0 *
                            sin(envObject->getOrientation() * PI / 180);
                    double new_y = envObject->getSize() / 2.0 *
                            cos(envObject->getOrientation() * PI / 180);

                    scene->addLine(envObject->getCoords().first / REAL_PIXEL_SIZE,
                                   envObject->getCoords().second / REAL_PIXEL_SIZE,
                                   (envObject->getCoords().first + new_x) / REAL_PIXEL_SIZE,
                                   (envObject->getCoords().second - new_y) / REAL_PIXEL_SIZE,
                                   QPen(outlineColor));
                }
            }
        }

        // Refresh robotWindows
        for (int i = 0; i < ROBOTS; i++) {
            robotWindows.at(i)->onRefreshMap();
        }

        QTimer::singleShot(1000 / SCREEN_REFRESH_RATE, this, SLOT(onRefreshMap()));
    } else {
        validateButtons(Stopped);
    }
}

void MainWindow::drawMap(QImage *image)
{
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(*image));
}


/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
