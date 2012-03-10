#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for (int i = 0; i < ROBOTS; i++) {
        RobotWindow* robotWindow = new RobotWindow;
        robotWindows.push_back(robotWindow);
        robotWindows.at(i)->setRobotId(i+1);
    }

    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);

    objects = new QVector<QGraphicsItem *>();

    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move(screen.center() - this->rect().center());
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

void MainWindow::on_closePushButton_clicked()
{
    on_action_Exit_triggered();
}

void MainWindow::on_action_Exit_triggered()
{
    //TODO: close all threads

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
        HubModule::modellingSystem =
                new ModellingSystem(loadMap(*image),
                                    std::pair<int, int>(image->height(), image->width()));
        delete image;

        std::pair<int, int> size = HubModule::modellingSystem->getWorld()->getSize();
        for (int i = 0; i < size.first; i++) {
            for (int j = 0; j < size.second; j++) {
                int *height = new int(HubModule::modellingSystem->getWorld()->getHeight(i, j));
                QColor *pixelColor = new QColor(*height, *height, *height);
                scene->addRect(i * REAL_PIXEL_SIZE, j * REAL_PIXEL_SIZE,
                               REAL_PIXEL_SIZE, REAL_PIXEL_SIZE,
                               QPen(*pixelColor), QBrush(*pixelColor));
                delete pixelColor;
                delete height;
            }
        }

        //TODO: start hub thread

        for (int i = 0; i < ROBOTS; i++) {
            robotWindows.at(i)->show();
        }

        QTimer::singleShot(0, this, SLOT(onRefreshMap()));

        //TODO: disable all buttons which can open map, start modelling process etc.
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

        // clear all objects before redrawing
        for (int i = 0; i < objects->size(); i++) {
            QGraphicsItem* item = objects->at(i);
            scene->removeItem(item);
            delete item;
        }
        objects->clear();

        //FIXME: Issue #1
        for (int i = 0; i < ROBOTS; i++) {
            Robot *robot = new Robot(HubModule::modellingSystem->getRobot(i));

            if (robot->getCoords().first >= (int)robot->getSize() / 2
                    && robot->getCoords().second >= (int)robot->getSize() / 2) {
                QColor outlineColor(255 - robot->getColor().red(),
                                    255 - robot->getColor().green(),
                                    255 - robot->getColor().blue());
                objects->push_back(scene->addEllipse(
                                       robot->getCoords().first - robot->getSize() / 2,
                                       robot->getCoords().second - robot->getSize() / 2,
                                       robot->getSize(), robot->getSize(),
                                       QPen(outlineColor), QBrush(robot->getColor()))
                                   );

                double new_x = robot->getSize() / 2.0 * sin(robot->getOrientation() * PI / 180);
                double new_y = robot->getSize() / 2.0 * cos(robot->getOrientation() * PI / 180);

                objects->push_back(scene->addLine(
                                       robot->getCoords().first, robot->getCoords().second,
                                       robot->getCoords().first + new_x,
                                       robot->getCoords().second - new_y,
                                       QPen(outlineColor))
                                   );
            }

            delete robot;
        }

        //FIXME: Issue #1
        for (int i = 0; i < ENV_OBJECTS; i++) {
            EnvObject *envObject = new EnvObject(HubModule::modellingSystem->getEnvObject(i));

            if (envObject->getCoords().first >= (int)envObject->getSize() / 2
                    && envObject->getCoords().second >= (int)envObject->getSize() / 2) {
                QColor outlineColor(255 - envObject->getColor().red(),
                                    255 - envObject->getColor().green(),
                                    255 - envObject->getColor().blue());
                objects->push_back(scene->addEllipse(
                                       envObject->getCoords().first - envObject->getSize() / 2,
                                       envObject->getCoords().second - envObject->getSize() / 2,
                                       envObject->getSize(), envObject->getSize(),
                                       QPen(outlineColor), QBrush(envObject->getColor()))
                                   );

                // draw orientation line if object is movable
                if (envObject->isMovable()) {
                    double new_x = envObject->getSize() / 2.0 *
                            sin(envObject->getOrientation() * PI / 180);
                    double new_y = envObject->getSize() / 2.0 *
                            cos(envObject->getOrientation() * PI / 180);

                    objects->push_back(scene->addLine(
                                           envObject->getCoords().first,
                                           envObject->getCoords().second,
                                           envObject->getCoords().first + new_x,
                                           envObject->getCoords().second - new_y,
                                           QPen(outlineColor))
                                       );
                }
            }

            delete envObject;
        }

        for (int i = 0; i < ROBOTS; i++) {
            robotWindows.at(i)->onRefreshMap();
        }

        QTimer::singleShot(1000 / SCREEN_REFRESH_RATE, this, SLOT(onRefreshMap()));
    } else {
        //TODO: enable all buttons which can open map, start modelling process etc.
    }
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
