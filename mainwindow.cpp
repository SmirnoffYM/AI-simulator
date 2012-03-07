#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QImage>

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

    //TODO: load bitmap to World
    //TODO: start all threads if loading successfull

    delete image;

    //TODO: close all threads
}

void MainWindow::on_actionAbout_Program_triggered()
{
    //TODO: description

    QMessageBox::information(this, tr("AI simulator"), tr("description..."));
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
