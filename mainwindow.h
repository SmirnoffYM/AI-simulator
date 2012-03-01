#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

private:
    Ui::MainWindow *ui;

    QList<RobotWindow*> robotWindows;
};

#endif // MAINWINDOW_H
