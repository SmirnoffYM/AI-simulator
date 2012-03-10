#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <QImage>
#include <QCloseEvent>
#include <QVector>
#include "robotwindow.h"
#include "constants.h"
#include "hubmodule.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_closePushButton_clicked();

    void on_action_Exit_triggered();

    void on_action_Open_map_triggered();

    void on_actionAbout_Program_triggered();

    void onRefreshMap();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QVector<QGraphicsItem *> *objects;

    QList<RobotWindow *> robotWindows;

    bool isMapCorrect(QImage image);
    int** loadMap(QImage image);

    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
