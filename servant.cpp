#include "servant.h"

Servant::Servant()
{
    applications = std::vector<std::pair<QString, QProcess *> >();
}

void Servant::addApplication(QString command)
{
    QProcess *application = new QProcess();
    applications.push_back(std::pair<QString, QProcess*>(command, application));
}

void Servant::launchApplications()
{
    for (unsigned int i = 0; i < applications.size(); i++) {
        applications.at(i).second->start(applications.at(i).first);
    }
}

void Servant::stopApplications()
{
    for (unsigned int i = 0; i < applications.size(); i++) {
        applications.at(i).second->kill();
    }
    applications.clear();
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
        if (!line.contains(QRegExp("^(//)")))
            configStringList.append(line);
    }
    config.close();

    // Check if port in filename is equals to port in file body
    int portFilename = configFilename.left(4).toInt();
    if (portFilename == 0 || portFilename != configStringList.at(1).toInt()) {
        qDebug() << "Ports in filename and file body aren't equals (robot" << number << ")";
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

    //TODO: parse and check local map size
    //TODO: parse and check local map scaling

    // Check intersection type
    QString intersection = configStringList.at(6);
    if (intersection != "0" && intersection != "1" && intersection != "2") {
        qDebug() << "Invalid intersection type (robot" << number << ")";
        return robot;
    }

    // Check orientation
    double orientation = configStringList.at(7).toDouble(&ok);
    if (!ok) {
        qDebug() << "Invalid orientation (robot" << number << ")";
        return robot;
    }

    // Check color
    QColor color = QColor(configStringList.at(8));
    if (!color.isValid()) {
        qDebug() << "Invalid color (robot" << number << ")";
        return robot;
    }
    Color transformedColor;
    transformedColor.red = color.red();
    transformedColor.green = color.green();
    transformedColor.blue = color.blue();

    // Check all custom parameters
    std::pair<std::string, double> *parameters =
            new std::pair<std::string, double>[CUSTOM_PARAMETERS_QUANTITY];
    for (int i = 0; i < CUSTOM_PARAMETERS_QUANTITY; i++) {
        QString line = configStringList.at(9+i);
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
    robot->setOrientation(orientation);
    robot->setColor(transformedColor);
    robot->setIntersection(static_cast<Intersection>(intersection.toInt()));
    robot->setParameters(parameters);

    QString command = configStringList.at(0) + QString(" ") + configFilename;
    qDebug() << "Robot" << number << "will be called by command" << command;
    addApplication(command);

    return robot;
}

// Draw all objects
// Each object is a circle colored with getColor()
// If object is movable, it has orientation
// Orientation is indicated by line with inverted circle color
// Line links circle's center and circle's outline
void Servant::drawObject(Object *object, QGraphicsScene *scene)
{
    if (object->getCoords().first >= static_cast<int>(object->getSize() / 2)
            && object->getCoords().second >= static_cast<int>(object->getSize() / 2)
            && object->getSize() > 0
            && object->getCoords().first + static_cast<int>(object->getSize() / 2) <=
            HubModule::modellingSystem->getWorld()->getSize().first * REAL_PIXEL_SIZE
            && object->getCoords().second + static_cast<int>(object->getSize() / 2) <=
            HubModule::modellingSystem->getWorld()->getSize().second * REAL_PIXEL_SIZE) {
        QColor outlineColor(255 - object->getColor().red,
                            255 - object->getColor().green,
                            255 - object->getColor().blue);

        int circle_x = (object->getCoords().first -
                        object->getSize() / 2) / REAL_PIXEL_SIZE;
        int circle_y = (object->getCoords().second -
                        object->getSize() / 2) / REAL_PIXEL_SIZE;

        scene->addEllipse(circle_x, circle_y,
                          object->getSize() / REAL_PIXEL_SIZE,
                          object->getSize() / REAL_PIXEL_SIZE,
                          QPen(outlineColor),
                          QBrush(Servant::getInstance().
                                 colorTransform(object->getColor())));

        // draw orientation line if object is movable
        if (object->isMovable()) {
            double new_x = object->getSize() / 2.0 *
                    sin(object->getOrientation() * PI / 180);
            double new_y = object->getSize() / 2.0 *
                    cos(object->getOrientation() * PI / 180);

            scene->addLine(object->getCoords().first / REAL_PIXEL_SIZE,
                           object->getCoords().second / REAL_PIXEL_SIZE,
                           (object->getCoords().first + new_x) / REAL_PIXEL_SIZE,
                           (object->getCoords().second - new_y) / REAL_PIXEL_SIZE,
                           QPen(outlineColor));
        }
    }
}

Color Servant::colorTransform(QColor col)
{
    Color color;
    color.red = col.red();
    color.green = col.green();
    color.blue = col.blue();
    return color;
}

QColor Servant::colorTransform(Color col)
{
    return QColor(col.red, col.green, col.blue);
}

QString Servant::getColorName(Color col)
{
    return colorTransform(col).name();
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
