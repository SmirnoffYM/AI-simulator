#include "modellingsystem.h"

bool ModellingSystem::isModellingPerformed = false;

ModellingSystem::ModellingSystem(int **map, std::pair<int, int> size)
{
    world = new World(map, size);

    for (int i = 0; i < ROBOTS; i++) {
        LoadRobotParameters(i);
    }

    for (int i = 0; i < ENV_OBJECTS; i++) {
        EnvObject *envObject = new EnvObject();

        //TODO: load envObject parameters from environment setting file

        envObjects.push_back(envObject);
    }

    isModellingPerformed = false;
}

ModellingSystem::~ModellingSystem()
{
    delete world;

    for (unsigned int i = 0; i < robots.size(); i++)
        delete robots.at(i);

    for (unsigned int i = 0; i < envObjects.size(); i++)
        delete envObjects.at(i);
}

// Load robot parameters from configuration file
// number must be in range 0..ROBOTS-1
// returns false if robot wasn't loaded
void ModellingSystem::LoadRobotParameters(unsigned int number)
{
    Robot *robot = new Robot();

    // Scan /robots/ directory for configuration files
    QStringList filters;
    filters << QString("????.%1").arg(number);
    QDir directory("robots/");
    QStringList files = directory.entryList(filters);

    // Choose only first file
    QString configFilename = "";
    for (int i = 0; i < files.size(); i++) {
        if (files.at(i).contains(QRegExp(QString("^(\\d){4}.%1$").arg(number)))) {
            configFilename = files.at(i);
            break;
        }
    }

    if (configFilename.isEmpty()) {
        qDebug() << "Cannot find configuration file for robot" << number;
        robots.push_back(robot);
        return;
    }

    // Load file contents (without commented strings) to configStringList
    QFile config(QString("robots/") + configFilename);
    QStringList configStringList = QStringList();
    if (!config.open(QFile::ReadOnly)) {
        qDebug() << "Cannot open first configuration file for robot" << number;
        robots.push_back(robot);
        return;
    }
    QTextStream stream (&config);
    QString line;
    while(!stream.atEnd()) {
        line = stream.readLine();
        if (!line.contains(QRegExp("^(//)")))
            configStringList.append(line);
    }
    config.close();

    /*
        ==========================
        Configuration file format
        ==========================
        0 - executable string
        1 - port number
        2 - robot size
        3 - local map size (currently isn't used)
        4 - local map scaling (currently isn't used)
        5 - intersection type (0, 1, 2)
        6 - orientation
        7 - robot color
        8..inf - custom robot parameters
    */

    //TODO: launch robot using executable string

    // Check if port in filename is equals to port in file body
    int portFilename = configFilename.left(4).toInt();

    if (portFilename == 0 || portFilename != configStringList.at(1).toInt()) {
        qDebug() << "Ports in filename and file body aren't equals (robot" << number << ")";
        robots.push_back(robot);
        return;
    }

    // Check if size is a number and is over than zero
    int size = configStringList.at(2).toInt();
    if (size <= 0) {
        qDebug() << "Invalid size (robot" << number << ")";
        robots.push_back(robot);
        return;
    }

    //TODO: parse and check local map size
    //TODO: parse and check local map scaling

    // Check intersection type
    QString intersection = configStringList.at(5);
    if (intersection != "0" && intersection != "1" && intersection != "2") {
        qDebug() << "Invalid intersection type (robot" << number << ")";
        robots.push_back(robot);
        return;
    }

    // Check orientation
    bool ok = true;
    double orientation = configStringList.at(6).toDouble(&ok);

    if (!ok) {
        qDebug() << "Invalid orientation (robot" << number << ")";
        robots.push_back(robot);
        return;
    }

    // Check color
    QColor color = QColor(configStringList.at(7));
    if (!color.isValid()) {
        qDebug() << "Invalid color (robot" << number << ")";
        robots.push_back(robot);
        return;
    }

    // TODO: Load and check all custom parameters

    robot->setSize(size);
    robot->setPortNumber(portFilename);
    robot->setOrientation(orientation);
    robot->setColor(color);
    robot->setIntersection(static_cast<Intersection>(intersection.toInt()));

    robots.push_back(robot);
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
