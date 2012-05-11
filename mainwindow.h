#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <QImage>
#include <QCloseEvent>
#include "robotwindow.h"
#include "constants.h"
#include "hubthread.h"
#include "servant.h"

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
    void on_action_Exit_triggered();

    void on_action_Open_map_triggered();

    void on_actionAbout_Program_triggered();

    void onRefreshMap();

    void on_actionRun_triggered();

    void on_actionStop_triggered();

    void on_actionPause_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QList<RobotWindow *> robotWindows;
    QImage *map;

    enum ButtonsState {Started, Paused, Stopped};

    bool mapOpened;             // this flag signals when map is already created and opened
    HubThread *hubThread;

    bool isMapCorrect(QImage image);
    int ** loadMap(QImage image);
    void drawMap(QImage *image);
    void stopModelling();
    void validateButtons(ButtonsState state);
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
