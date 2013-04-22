#include "servant.h"
#include "processcontainer.h"
#include <QSettings>

Servant::Servant()
{
}

Robot * Servant::buildRobot(unsigned int number)
{
    Robot *robot = new Robot();

    // Scan /robots/ directory for configuration files
    QStringList filters;
    filters << QString("????.%1.ini").arg(number);
    QDir directory("robots/");
    QStringList files = directory.entryList(filters);

    // Choose only first file
    QString configFilename = "";
    for (int i = 0; i < files.size(); i++) {
        if (files.at(i).contains(QRegExp(QString("^(\\d){4}.%1.ini$").arg(number)))) {
            configFilename = files.at(i);
            break;
        }
    }

    if (configFilename.isEmpty()) {
        qDebug() << "Cannot find configuration file for robot" << number;
        return robot;
    }

    QSettings settings("robots/" + configFilename, QSettings::IniFormat);

    /* Robot main parameters */
    settings.beginGroup("robot");
    int x = settings.value("startX").toInt();
    int y = settings.value("startY").toInt();
    int size = settings.value("size").toInt();
    int portNumber = settings.value("port").toInt();
    int type = settings.value("type").toInt();
    int visibilityRadius = settings.value("visibilityRadius").toInt();
    int visibilityAngle = settings.value("visibilityAngle").toInt();
    int orientation = settings.value("orientation").toInt();
    QColor color = QColor(settings.value("color").toString());
    int intersection = settings.value("intersection").toInt();
    int scaling = settings.value("localMapScaling").toInt();
    settings.endGroup();

    /* Custom parameters */
    settings.beginGroup("robot_custom_params");
    std::pair<std::string, double> parameters[CUSTOM_PARAMETERS_QUANTITY];

    const QStringList childKeys = settings.childKeys();
    for (int i = 0; i < CUSTOM_PARAMETERS_QUANTITY; i++) {
        parameters[i].first = childKeys.at(i).toStdString();
        parameters[i].second = settings.value(childKeys.at(i)).toInt();
    }
    settings.endGroup();

    robot->setCoords(x, y);
    robot->setSize(size);
    robot->setPortNumber(portNumber);
    robot->setType(static_cast<RobotType>(type));
    robot->setVisibilityRadius(visibilityRadius);
    robot->setVisibilityAngle(visibilityAngle);
    robot->setOrientation(orientation);
    robot->setColor(Color(color.red(), color.green(), color.blue()));
    robot->setIntersection(static_cast<Intersection>(intersection));
    robot->setParameters(parameters);

    this->scaling[number] = scaling;

    // TODO: fix this shit - configFilename.left(6)
    QString command = settings.value("robot/launch").toString() + QString(" ") + configFilename.left(6);
    qDebug() << "Robot" << number << "will be called by command" << command;
    ProcessContainer::getInstance().addApplication(command);

    return robot;
}

std::vector<EnvObject *> Servant::buildEnvironment(std::pair<int, int> mapSize)
{
    std::vector<EnvObject *> *environment = new std::vector<EnvObject *>();

    // Scan /robots/ directory for configuration files
    QStringList filters;
    filters << QString("????.env");
    QDir directory("robots/");
    QStringList files = directory.entryList(filters);

    // Choose only first file
    QString configFilename = "";
    for (int i = 0; i < files.size(); i++) {
        if (files.at(i).contains(QRegExp(QString("^(\\d){4}.env$")))) {
            configFilename = files.at(i);
            break;
        }
    }

    if (configFilename.isEmpty()) {
        qDebug() << "Cannot find configuration file for the environment";
        return *environment;
    }

    // Load file contents (without commented strings) to configStringList
    QFile config(QString("robots/") + configFilename);
    QStringList configStringList = QStringList();
    if (!config.open(QFile::ReadOnly)) {
        qDebug() << "Cannot open configuration file for the environment";
        return *environment;
    }
    QTextStream stream (&config);
    QString line;
    while(!stream.atEnd()) {
        line = stream.readLine();
        if (!line.contains(QRegExp("^(//)")) && !line.isEmpty())
            configStringList.append(line);
    }
    config.close();

    // check launch command
    if (configStringList.at(0).isEmpty()) {
        qDebug() << "Launch command is empty!";
        return *environment;
    }

    // check port
    int portFilename = configFilename.left(4).toInt();
    if (portFilename == 0 || portFilename != configStringList.at(1).toInt()) {
        qDebug() << "Ports in filename and file body aren't equal!";
        return *environment;
    }
    EnvObject::setPortNumber(portFilename);

    // start parsing each object
    const int parametersQuantityPerObject = 8;      // See AI-simulator wiki
    QVector<int> indexes = QVector<int>();
    for (int obj = 0; obj < ENV_OBJECTS; obj++) {
        EnvObject *envObject = new EnvObject();

        if (configStringList.size() < 2 + (obj+1) * parametersQuantityPerObject)
            break;
        QStringList objectParams = QStringList();
        for (int i = 0; i < parametersQuantityPerObject; i++)
            objectParams.push_back(configStringList.at(2 + obj * parametersQuantityPerObject + i));
        bool ok = true;

        // Check object id
        int index = objectParams.at(0).toInt(&ok);
        if (!ok || index < 0) {
            qDebug() << "Id must be non-negative integer number (object" << obj << ")";
            return * environment;
        }
        if (indexes.contains(index)) {
            qDebug() << "Object with same id already exists (object" << obj << ")";
            return *environment;
        }

        // Check object start position
        QString pos = objectParams.at(1);
        if (!pos.contains(QRegExp("^(\\d)+;(\\d)+$")) && pos != QString("-1;-1")) {
            qDebug() << "Invalid start position (object" << obj << ")";
            return *environment;
        }
        int x, y;
        if (pos != QString("-1;-1"))
        {
            x = pos.split(";").at(0).toInt(&ok);
            if (!ok) {
                qDebug() << "Invalid start position (object" << obj << ")";
                return *environment;
            }
            y = pos.split(";").at(1).toInt(&ok);
            if (!ok) {
                qDebug() << "Invalid start position (object" << obj << ")";
                return *environment;
            }
            if (x < 0 || y < 0
                    || x >= mapSize.first * REAL_PIXEL_SIZE
                    || y >= mapSize.second * REAL_PIXEL_SIZE) {
                qDebug() << "Start position is out of the map (object" << obj << ")";
                return *environment;
            }
        } else {
            /*
            srand(static_cast<unsigned int>(time(0)));
            x = rand() % (mapSize.first * REAL_PIXEL_SIZE);
            y = rand() % (mapSize.second * REAL_PIXEL_SIZE);
            qDebug() << "Object" << obj << "receives random coordinates (" << x << "," << y << ")";
            */
            // coordinates must be generated in the environment app
            x = 0;
            y = 0;
        }

        // Check if size is a number and is over than zero
        int size = objectParams.at(2).toInt();
        if (size <= 0) {
            qDebug() << "Invalid size (object" << obj << ")";
            return *environment;
        }

        // Check intersection type
        QString intersection = objectParams.at(3);
        if (intersection != "0" && intersection != "1" && intersection != "2") {
            qDebug() << "Invalid intersection type (object" << obj << ")";
            return *environment;
        }

        bool movable;
        if (objectParams.at(4) == QString("0"))
            movable = false;
        else if (objectParams.at(4) == QString("1"))
            movable = true;
        else {
            qDebug() << "Movable parameter can receive only 0 or 1 (object" << obj << ")";
            return *environment;
        }

        // Check orientation
        int orientation = objectParams.at(5).toDouble(&ok);
        if (!ok || orientation < 0) {
            qDebug() << "Invalid orientation (object" << obj << ")";
            return *environment;
        }

        int velocity = objectParams.at(6).toInt();
        if (velocity <= 0) {
            qDebug() << "Invalid velocity (object" << obj << ")";
            return *environment;
        }

        // Check color
        QColor color = QColor(objectParams.at(7));
        if (!color.isValid()) {
            qDebug() << "Invalid color (object" << obj << ")";
            return *environment;
        }

        indexes.push_back(index);
        envObject->setObjectId(index);
        envObject->setCoords(x, y);
        envObject->setSize(size);
        envObject->setIntersection(static_cast<Intersection>(intersection.toInt()));
        envObject->setMovable(movable);
        envObject->setOrientation(orientation);
        envObject->setVelocity(velocity);
        envObject->setColor(Color(color.red(), color.green(), color.blue()));
        environment->push_back(envObject);
    }

    QString command = configStringList.at(0) + QString(" ") + configFilename + QString(" ")
            + QString("%1").arg(mapSize.first) + QString(" ") + QString("%1").arg(mapSize.second);
    qDebug() << "Environment will be called by command" << command;
    //ProcessContainer::getInstance().addApplication(command);

    return *environment;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
