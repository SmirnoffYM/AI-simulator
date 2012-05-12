#include "servant.h"
#include "processcontainer.h"

Servant::Servant()
{
}

Robot * Servant::buildRobot(unsigned int number)
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
        return robot;
    }

    // Load file contents (without commented strings) to configStringList
    QFile config(QString("robots/") + configFilename);
    QStringList configStringList = QStringList();
    if (!config.open(QFile::ReadOnly)) {
        qDebug() << "Cannot open configuration file for robot" << number;
        return robot;
    }
    QTextStream stream (&config);
    QString line;
    while(!stream.atEnd()) {
        line = stream.readLine();
        if (!line.contains(QRegExp("^(//)")) && !line.isEmpty())
            configStringList.append(line);
    }
    config.close();

    if (configStringList.at(0).isEmpty()) {
        qDebug() << "Launch command is empty (robot" << number << ")";
        return robot;
    }

    // Check if port in filename is equals to port in file body
    int portFilename = configFilename.left(4).toInt();
    if (portFilename == 0 || portFilename != configStringList.at(1).toInt()) {
        qDebug() << "Ports in filename and file body aren't equal (robot" << number << ")";
        return robot;
    }

    // Check start position
    QString pos = configStringList.at(2);
    if (!pos.contains(QRegExp("^(\\d)+;(\\d)+$"))) {
        qDebug() << "Invalid start position (robot" << number << ")";
        return robot;
    }
    bool ok = true;
    int x = pos.split(";").at(0).toInt(&ok);
    if (!ok) {
        qDebug() << "Invalid start position (robot" << number << ")";
        return robot;
    }
    int y = pos.split(";").at(1).toInt(&ok);
    if (!ok) {
        qDebug() << "Invalid start position (robot" << number << ")";
        return robot;
    }

    // Check if size is a number and is over than zero
    int size = configStringList.at(3).toInt();
    if (size <= 0) {
        qDebug() << "Invalid size (robot" << number << ")";
        return robot;
    }

    // Check robot type
    int type = configStringList.at(4).toInt(&ok);
    if (!ok || (type != 0 && type != 1)) {
        qDebug() << "Invalid robot type (robot" << number << ")";
        return robot;
    }

    // Check visibility radius
    int visibilityRadius = configStringList.at(5).toInt(&ok);
    if (!ok || visibilityRadius < 0) {
        qDebug() << "Invalid visibilty radius (robot" << number << ")";
        return robot;
    }

    double visibilityAngle = configStringList.at(6).toDouble(&ok);
    if (!ok || visibilityAngle <= 0 || visibilityAngle > 360) {
        qDebug() << "Invalid visibility angle (robot" << number << ")";
        return robot;
    }

    // Check local map scaling
    int scaling = configStringList.at(7).toInt(&ok);
    if (!ok || scaling < 1) {
        qDebug() << "Invalid local map scaling (robot" << number << ")";
        return robot;
    }

    // Check intersection type
    QString intersection = configStringList.at(8);
    if (intersection != "0" && intersection != "1" && intersection != "2") {
        qDebug() << "Invalid intersection type (robot" << number << ")";
        return robot;
    }

    // Check orientation
    double orientation = configStringList.at(9).toDouble(&ok);
    if (!ok || orientation < 0) {
        qDebug() << "Invalid orientation (robot" << number << ")";
        return robot;
    }

    // Check color
    QColor color = QColor(configStringList.at(10));
    if (!color.isValid()) {
        qDebug() << "Invalid color (robot" << number << ")";
        return robot;
    }

    // Check all custom parameters
    std::pair<std::string, double> *parameters =
            new std::pair<std::string, double>[CUSTOM_PARAMETERS_QUANTITY];
    for (int i = 0; i < CUSTOM_PARAMETERS_QUANTITY; i++) {
        QString line = configStringList.at(11+i);
        if (!line.contains(QRegExp("^(\\d|\\.)+;(\\w|\\s)+$"))) {
            qDebug() << "Invalid parameter" << i+1 << "(robot" << number << ")";
            return robot;
        }
        double value = line.left(line.indexOf(QString(";"))).toDouble(&ok);
        if (!ok) {
            qDebug() << "Invalid value of parameter" << i+1 << "(robot" << number << ")";
            return robot;
        }
        std::string name = line.mid(line.indexOf(QString(";")) + 1).toStdString();
        parameters[i] = std::pair<std::string, double>(name, value);
    }

    robot->setCoords(x, y);
    robot->setSize(size);
    robot->setPortNumber(portFilename);
    robot->setType(static_cast<RobotType>(type));
    robot->setVisibilityRadius(visibilityRadius);
    robot->setVisibilityAngle(visibilityAngle);
    robot->setOrientation(orientation);
    robot->setColor(Color(color.red(), color.green(), color.blue()));
    robot->setIntersection(static_cast<Intersection>(intersection.toInt()));
    robot->setParameters(parameters);

    this->scaling[number] = scaling;

    QString command = configStringList.at(0) + QString(" ") + configFilename;
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
            srand(static_cast<unsigned int>(time(0)));
            x = rand() % (mapSize.first * REAL_PIXEL_SIZE);
            y = rand() % (mapSize.second * REAL_PIXEL_SIZE);
            qDebug() << "Object" << obj << "receives random coordinates (" << x << "," << y << ")";
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
        double orientation = objectParams.at(5).toDouble(&ok);
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

    QString command = configStringList.at(0) + QString(" ") + configFilename;
    qDebug() << "Environment will be called by command" << command;
    ProcessContainer::getInstance().addApplication(command);

    return *environment;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
