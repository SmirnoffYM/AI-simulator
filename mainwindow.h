#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <QImage>
#include <QCloseEvent>
#include <QThread>
#include "hubmodule.h"
#include "robotwindow.h"
#include "constants.h"
#include "mapgraphicsscene.h"
#include "processcontainer.h"
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
    MapGraphicsScene *scene;
    QList<RobotWindow *> robotWindows;
    QImage *map;

    bool mapOpened;             // this flag signals when map is already created and opened
    QThread *hubThread;
    HubModule *hub;

    bool isMapCorrect(QImage image);
    int ** loadMap(QImage image);
    void drawMap(QImage *image);
    void stopModelling();
    void validateButtons(ModellingState state);
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
