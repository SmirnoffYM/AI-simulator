#include "messagehandler.h"
#include "modellingsystem.h"
#include "hubmodule.h"
#include "messages.h"
#include <cmath>
#include <QDebug>

MessageHandler::MessageHandler(ComModule *comModule)
{
    this->comModule = comModule;
}

void MessageHandler::handle(Message *msg)
{
    switch (msg->type) {
    case MsgMove:
        handle(static_cast<MessageMove *> (msg));
        break;

    case MsgTurn:
        handle(static_cast<MessageTurn *> (msg));
        break;

    case MsgChangeSize:
        handle(static_cast<MessageChangeSize *> (msg));
        break;

    case MsgChangeColor:
        handle(static_cast<MessageChangeColor *> (msg));
        break;

    case MsgWhoIsThere:
        handle(static_cast<MessageWhoIsThere *> (msg));
        break;

    case MsgParameterReport:
        handle(static_cast<MessageParameterReport *> (msg));
        break;

    default:
        // agent can't send any other message types, thus we don't need to handle them
        break;
    }
}

// handle MessageMove message
void MessageHandler::handle(MessageMove *msg)
{
    // check if there'll be no collisions
    bool collision = false;

    Object* object = NULL;

    if (msg->envObjID == 0) {
        object = HubModule::modellingSystem
                ->getRobotByPort(msg->port);
    } else {
        object = HubModule::modellingSystem
                ->getEnvObject(msg->envObjID - 1);
    }


    int degrees = 360 - object->getOrientation();
    double radians = degrees * (PI / 180);

    int destX = object->getCoords().first
            + msg->coordX * cos(radians) - msg->coordY * sin(radians);
    int destY = object->getCoords().second
            - msg->coordX * sin(radians) - msg->coordY * cos(radians);


    // REFACTORING!!!! WHERE ARE YOU?
    int bumpX;
    int bumpY;


    // check for collisions with robots
    MessageBump *messageBump = new MessageBump();

    // check for collisions with border
    std::pair<int, int> border = HubModule::modellingSystem->getWorld()->getSize();
    border.first *= REAL_PIXEL_SIZE;
    border.second *= REAL_PIXEL_SIZE;

    if ((destX + object->getSize() / 2 > static_cast<double>(border.first)) ||
            (destX - (int) object->getSize() / 2 < 0) ||
            (destY + object->getSize() / 2 > static_cast<double>(border.second)) ||
            (destY - (int) object->getSize() / 2 < 0)) {
        collision = true;
        bumpX = msg->coordX;
        bumpY = msg->coordY;
        messageBump->bumpType = 0;
    }

    for (int i = 0; i < ROBOTS; i++) {
        Robot* tmpRobot = HubModule::modellingSystem->getRobot(i);
        // if the current robot is the sender robot
        // then make the next loop
        if (tmpRobot != NULL
                && tmpRobot->getPortNumber() == msg->port)
            continue;

        // check if distance between two points is
        // bigger then robots size sum
        if (tmpRobot != NULL
                && tmpRobot->getSize() != 0
                && sqrt(
                    pow((int)(destX
                              - tmpRobot->getCoords().first), 2)
                    +
                    pow((int)(destY
                              - tmpRobot->getCoords().second), 2)
                    ) < (tmpRobot->getSize() / 2 + object->getSize() / 2)
                ) {
            collision = true;
            bumpX = tmpRobot->getCoords().first;
            bumpY = tmpRobot->getCoords().second;
            messageBump->bumpType = 1;
            // send message to collided robot
            Message *m = new Message();
            // why 0? read envObjID specification
            m->envObjID = 0;
            m->port = tmpRobot->getPortNumber();
            // just dummy
            m->num = 0;
            m->type = MsgHit;

            comModule->sendMessage(m);
        }
    }
    // check for collisions with env objects
    for (int i = 0; i < ENV_OBJECTS; i++) {
        EnvObject* tmpEnvObject = HubModule::modellingSystem->getEnvObject(i);
        // if the current env object is the sender env object
        // then make the next loop
        if (tmpEnvObject != NULL
                && tmpEnvObject->getObjectId() == (msg->envObjID - 1))
            continue;
        // check if distance between two points is
        // bigger then robots size sum
        if (tmpEnvObject != NULL
                && tmpEnvObject->getSize() != 0
                && sqrt(
                    pow((int)(destX
                              - tmpEnvObject->getCoords().first), 2)
                    +
                    pow((int)(destY
                              - tmpEnvObject->getCoords().second), 2)
                    ) < (tmpEnvObject->getSize() / 2 + object->getSize() / 2)
                ) {
            collision = true;
            bumpX = tmpEnvObject->getCoords().first;
            bumpY = tmpEnvObject->getCoords().second;
            messageBump->bumpType = 1;
            // send message to collided robot
            Message *m = new Message();
            // why 0? read envObjID specification
            m->envObjID = i + 1;
            m->port = tmpEnvObject->getPortNumber();
            // just dummy
            m->num = 0;
            m->type = MsgHit;

            comModule->sendMessage(m);
        }
    }

    if (!collision)
        if (msg->envObjID == 0) {
            HubModule::modellingSystem->getRobotByPort(msg->port)
                    ->setCoords(destX, destY);
            Message *m = new Message();
            m->type = MsgMovedSuccessfully;
            m->port = msg->port;
            m->envObjID = 0;
            m->num = 0;

            comModule->sendMessage(m);
        }
        else {
            HubModule::modellingSystem->getEnvObject(msg->envObjID - 1)
                    ->setCoords(destX, destY);
            Message *m = new Message();
            m->type = MsgMovedSuccessfully;
            m->port = msg->port;
            m->envObjID = msg->envObjID;
            m->num = 0;

            comModule->sendMessage(m);
        }
    else {
        messageBump->port = msg->port;
        messageBump->envObjID = msg->envObjID;
        messageBump->num = msg->num;
        // set bump object coords
        int x = object->getCoords().first * cos(radians)
                + object->getCoords().second * sin(radians);
        int y = - object->getCoords().first * sin(radians)
                + object->getCoords().second * cos(radians);

        int objX = bumpX * cos(radians) + bumpY * sin(radians);
        int objY = - bumpX * sin(radians) + bumpY * cos(radians);

        messageBump->coordX = objX - x;
        messageBump->coordY = y - objY;

        comModule->sendMessage(messageBump);
    }
}

// handle MessageTurn message
void MessageHandler::handle(MessageTurn *msg)
{
    if (msg->envObjID == 0) {
        Robot *robot = HubModule::modellingSystem->getRobotByPort(msg->port);
        unsigned int currentOrientation = robot->getOrientation();

        robot->setOrientation(currentOrientation + msg->degrees);
    }
    else {
        EnvObject* env = HubModule::modellingSystem->getEnvObject(msg->envObjID - 1);
        unsigned int currentOrientation = env->getOrientation();

        env->setOrientation(currentOrientation + msg->degrees);
    }
}

// handle MessageChangeSize message
void MessageHandler::handle(MessageChangeSize *msg)
{
    if (msg->envObjID == 0)
        HubModule::modellingSystem->getRobotByPort(msg->port)
                ->setSize(msg->diameter);
    else
        HubModule::modellingSystem->getEnvObject(msg->envObjID - 1)
                ->setSize(msg->diameter);
}

// handle MessageChangeColor message
void MessageHandler::handle(MessageChangeColor *msg)
{
    if (msg->envObjID == 0)
        HubModule::modellingSystem->getRobotByPort(msg->port)
                ->setColor(Color(msg->red,
                                 msg->green,
                                 msg->blue));
    else
        HubModule::modellingSystem->getEnvObject(msg->envObjID - 1)
                ->setColor(Color(msg->red,
                                 msg->green,
                                 msg->blue));
}

// handle MessageWhoIsThere message
void MessageHandler::handle(MessageWhoIsThere *msg)
{
    // we need to send message about objects in
    // received range, so we must create MessageThereYouSee message
    MessageThereYouSee *messageThereYouSee = new MessageThereYouSee();

    // we also need a list of those objects
    std::list<MessageObject> objectsInRange;

    Robot *robot = HubModule::modellingSystem->getRobotByPort(msg->port);

    std::vector<Object *> objVector = std::vector<Object *>();
    objVector = robot->iCanSee();

    double degrees = 360 - robot->getOrientation();
    double radians = degrees * (PI / 180);

    int x = robot->getCoords().first * cos(radians)
            + robot->getCoords().second * sin(radians);
    int y = - robot->getCoords().first * sin(radians)
            + robot->getCoords().second * cos(radians);

    for (unsigned int i = 0; i < objVector.size(); i++) {
        MessageObject messageObject;
        Object* obj = objVector.at(i);
        // set color
        messageObject.type = 1;
        messageObject.red = obj->getColor().red();
        messageObject.green = obj->getColor().green();
        messageObject.blue = obj->getColor().blue();
        // set coordinates
        int objX = obj->getCoords().first * cos(radians)
                + obj->getCoords().second * sin(radians);
        int objY = - obj->getCoords().first * sin(radians)
                + obj->getCoords().second * cos(radians);

        messageObject.coordX = objX - x;
        messageObject.coordY = y - objY;
        // set diameter
        messageObject.diameter = objVector.at(i)->getSize();
        // set orientation
        messageObject.degrees = objVector.at(i)->getOrientation();

        objectsInRange.push_front(messageObject);
    }

    objVector.clear();

    messageThereYouSee->objects = objectsInRange;
    messageThereYouSee->port = msg->port;
    messageThereYouSee->num = msg->num;
    // send message to robot
    comModule->sendMessage(messageThereYouSee);
}

// handle MessageParameterReport message
void MessageHandler::handle(MessageParameterReport *msg)
{
    // hmm..need to remember what 1000000 means and
    // add it to constants.h :(
    double paramValue = msg->integral + msg->real
            / 1000000;

    HubModule::modellingSystem->getRobotByPort(msg->port)
            ->setParametersByID(msg->id, paramValue);
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
