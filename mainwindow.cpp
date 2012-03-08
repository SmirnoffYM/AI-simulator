#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for (int i = 0; i < ROBOTS; i++) {
        RobotWindow* robotWindow = new RobotWindow;
        robotWindows.push_back(robotWindow);
    }

    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);

    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move(screen.center() - this->rect().center());
}

MainWindow::~MainWindow()
{
    delete ui;
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

        //TODO: start hub thread

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
        scene->clear();

        /*
        std::pair<int, int> size = HubModule::modellingSystem->getWorld().getSize();
        for (int i = 0; i < size.first; i++) {
            for (int j = 0; j < size.second; j++) {
                int *height = new int(HubModule::modellingSystem->getWorld().getHeight(i, j));
                QColor *pixelColor = new QColor(*height, *height, *height);
                scene->addRect(i * REAL_PIXEL_SIZE, j * REAL_PIXEL_SIZE,
                               (i+1) * REAL_PIXEL_SIZE, (j+1) * REAL_PIXEL_SIZE,
                               QPen(*pixelColor), QBrush(*pixelColor));
                delete pixelColor;
                delete height;
            }
        }
        */

        //TODO: refresh map
        //TODO: refersh local maps in robot windows

        QTimer::singleShot(1000 / SCREEN_REFRESH_RATE, this, SLOT(onRefreshMap()));
    } else {
        //TODO: enable all buttons which can open map, start modelling process etc.
    }
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
