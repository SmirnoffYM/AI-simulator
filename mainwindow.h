#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "robotwindow.h"
#include "constants.h"
#include "hubmodule.h"
#include "qhubthread.h"
#include "qcomthread.h"

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

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;

    QList<RobotWindow*> robotWindows;
    QHubThread* hubThread;
    QComThread* comThread;
};

#endif // MAINWINDOW_H
