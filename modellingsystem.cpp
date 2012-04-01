#include "modellingsystem.h"
#include "hubmodule.h"

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
        qDebug() << "Cannot open configuration file for robot" << number;
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

    // Check if port in filename is equals to port in file body
    int portFilename = configFilename.left(4).toInt();
    if (portFilename == 0 || portFilename != configStringList.at(1).toInt()) {
        qDebug() << "Ports in filename and file body aren't equals (robot" << number << ")";
        robots.push_back(robot);
        return;
    }

    // Check start position
    QString pos = configStringList.at(2);
    if (!pos.contains(QRegExp("^(\\d)+;(\\d)+$"))) {
        qDebug() << "Invalid start position (robot" << number << ")";
        robots.push_back(robot);
        return;
    }
    bool ok = true;
    int x = pos.split(";").at(0).toInt(&ok);
    if (!ok) {
        qDebug() << "Invalid start position (robot" << number << ")";
        robots.push_back(robot);
        return;
    }
    int y = pos.split(";").at(1).toInt(&ok);
    if (!ok) {
        qDebug() << "Invalid start position (robot" << number << ")";
        robots.push_back(robot);
        return;
    }

    // Check if size is a number and is over than zero
    int size = configStringList.at(3).toInt();
    if (size <= 0) {
        qDebug() << "Invalid size (robot" << number << ")";
        robots.push_back(robot);
        return;
    }

    //TODO: parse and check local map size
    //TODO: parse and check local map scaling

    // Check intersection type
    QString intersection = configStringList.at(6);
    if (intersection != "0" && intersection != "1" && intersection != "2") {
        qDebug() << "Invalid intersection type (robot" << number << ")";
        robots.push_back(robot);
        return;
    }

    // Check orientation
    double orientation = configStringList.at(7).toDouble(&ok);
    if (!ok) {
        qDebug() << "Invalid orientation (robot" << number << ")";
        robots.push_back(robot);
        return;
    }

    // Check color
    QColor color = QColor(configStringList.at(8));
    if (!color.isValid()) {
        qDebug() << "Invalid color (robot" << number << ")";
        robots.push_back(robot);
        return;
    }

    // Check all custom parameters
    std::pair<std::string, double> *parameters =
            new std::pair<std::string, double>[CUSTOM_PARAMETERS_QUANTITY];
    for (int i = 0; i < CUSTOM_PARAMETERS_QUANTITY; i++) {
        QString line = configStringList.at(9+i);
        if (!line.contains(QRegExp("^(\\d|\\.)+;(\\w|\\s)+$"))) {
            qDebug() << "Invalid parameter" << i+1 << "(robot" << number << ")";
            robots.push_back(robot);
            return;
        }
        double value = line.left(line.indexOf(QString(";"))).toDouble(&ok);
        if (!ok) {
            qDebug() << "Invalid value of parameter" << i+1 << "(robot" << number << ")";
            robots.push_back(robot);
            return;
        }
        std::string name = line.mid(line.indexOf(QString(";")) + 1).toStdString();
        parameters[i] = std::pair<std::string, double>(name, value);
    }

    robot->setCoords(x, y);
    robot->setSize(size);
    robot->setPortNumber(portFilename);
    robot->setOrientation(orientation);
    robot->setColor(color);
    robot->setIntersection(static_cast<Intersection>(intersection.toInt()));
    robot->setParameters(parameters);

    QString command = configStringList.at(0) + QString(" ") + configFilename;
    qDebug() << "Robot" << number << "will be called by command" << command;
    HubModule::AddApplication(command);

    robots.push_back(robot);
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
