#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_closePushButton_clicked()
{
    this->close();
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
    QImage *image = new QImage(fileName);

    if (isMapCorrect(*image)) {
        std::vector<std::vector<Cell> > heightsMap = loadMap(*image);
        HubModule::modellingSystem = new ModellingSystem(heightsMap);
        delete image;

        //TODO: start hub thread

        //TODO: update map every SCREEN_REFRESH_RATE times per second

        //TODO: close hub thread
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

std::vector<std::vector<Cell> > MainWindow::loadMap(QImage image)
{
    std::vector<std::vector<Cell> > map;

    for (int i = 0; i < image.height(); i++) {
        std::vector<Cell> cellsRow;
        for (int j = 0; j < image.width(); j++) {
            Cell cell;
            cell.x = i;
            cell.y = j;
            cell.height = qGray(image.pixel(i, j));
            cellsRow.push_back(cell);
        }
        map.push_back(cellsRow);
    }

    return map;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
