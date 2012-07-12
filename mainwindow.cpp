#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new MapGraphicsScene();
    ui->graphicsView->setScene(scene);

    const QRect screen = QApplication::desktop()->screenGeometry();

    this->showMaximized();

    for (int i = 0; i < ROBOTS; i++)
    {

        RobotWindow *robotWindow = new RobotWindow;

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

    mapOpened = false;
    map = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (HubModule::modellingSystem->modellingState == Started)
        stopModelling();
    for (int i = 0; i < ROBOTS; i++) {
        robotWindows.at(i)->setClosePermit(true);
        robotWindows.at(i)->close();
    }
}

void MainWindow::on_action_Exit_triggered()
{
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
    if (HubModule::modellingSystem->modellingState == Stopped) {

        // if modelling was stopped and then started again
        if (HubModule::modellingSystem != NULL && !mapOpened) {

            int **heightMap = HubModule::modellingSystem->getWorld()->getHeightsMap();
            std::pair<int, int> size = HubModule::modellingSystem->getWorld()->getSize();
            HubModule::modellingSystem->~ModellingSystem();
            HubModule::modellingSystem = new ModellingSystem(heightMap, size);
            drawMap(map);
        }

        // starting hub thread
        hubThread = new HubThread();
        hubThread->start();
        ProcessContainer::getInstance().launchApplications();

        for (int i = 0; i < ROBOTS; i++) {
            robotWindows.at(i)->setMap(map);
            robotWindows.at(i)->setScaling(Servant::getInstance().getScaling(i));
        }
    }

    HubModule::modellingSystem->modellingState = Started;
    HubModule::modellingSystem->isModellingStateChanged = true;
    validateButtons(Started);
    QTimer::singleShot(0, this, SLOT(onRefreshMap()));
}

void MainWindow::on_actionPause_triggered()
{
    HubModule::modellingSystem->modellingState = Paused;
    HubModule::modellingSystem->isModellingStateChanged = true;
    validateButtons(Paused);
}

void MainWindow::on_actionStop_triggered()
{
    HubModule::modellingSystem->modellingState = Stopped;
    HubModule::modellingSystem->isModellingStateChanged = true;
    mapOpened = false;
    stopModelling();
    validateButtons(Stopped);
}

void MainWindow::stopModelling()
{
    for (int i = 0; i < ROBOTS; i++) {
        robotWindows.at(i)->hide();
    }

    ProcessContainer::getInstance().stopApplications();
    hubThread->terminate();
    hubThread->~HubThread();
}

void MainWindow::validateButtons(ModellingState state)
{
    switch(state) {
    case Started:
        ui->action_Open_map->setEnabled(false);
        ui->actionRun->setEnabled(false);
        ui->actionStop->setEnabled(true);
        ui->actionPause->setEnabled(true);
        break;
    case Paused:
        ui->action_Open_map->setEnabled(false);
        ui->actionRun->setEnabled(true);
        ui->actionStop->setEnabled(true);
        ui->actionPause->setEnabled(false);
        break;
    case Stopped:
        ui->action_Open_map->setEnabled(true);
        ui->actionRun->setEnabled(true);
        ui->actionStop->setEnabled(false);
        ui->actionPause->setEnabled(false);
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
    if (ModellingSystem::modellingState == Started) {

        // hide non active robot windows if any
        double *idleTime = HubModule::getIdleTime();
        for (int i = 0; i < ROBOTS; i++)
            if (idleTime[i] < ROBOT_TIMEOUT && ! robotWindows[i]->isVisible())
                robotWindows[i]->show();

        drawMap(map);
        for (int i = 0; i < ROBOTS; i++) {
            Object *robot = HubModule::modellingSystem->getRobot(i);
            if (robot != NULL)
                scene->drawObject(robot);
        }
        for (int i = 0; i < ENV_OBJECTS; i++) {
            Object *envObject = HubModule::modellingSystem->getEnvObject(i);
            if (envObject != NULL)
                scene->drawObject(envObject);
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
