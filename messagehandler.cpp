#include "messagehandler.h"
#include "modellingsystem.h"
#include "hubmodule.h"
#include "messages.h"
#include <cmath>

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

    if (msg->envObjID == 0)
        object = HubModule::modellingSystem
                ->getRobotByPort(msg->port);
    else
        object = HubModule::modellingSystem
                ->getEnvObject(msg->envObjID - 1);


    // check for collisions with robots
    MessageBump *messageBump = new MessageBump();

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
                    pow((int)(msg->coordX
                              - tmpRobot->getCoords().first), 2)
                    +
                    pow((int)(msg->coordY
                              - tmpRobot->getCoords().second), 2)
                    ) < (tmpRobot->getSize() / 2 + object->getSize() / 2)
                ) {
            collision = true;
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
                    pow((int)(msg->coordX
                              - tmpEnvObject->getCoords().first), 2)
                    +
                    pow((int)(msg->coordY
                              - tmpEnvObject->getCoords().second), 2)
                    ) < (tmpEnvObject->getSize() / 2 + object->getSize() / 2)
                ) {
            collision = true;
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
                    ->setCoords(msg->coordX, msg->coordY);
            Message *m = new Message();
            m->type = MsgMovedSuccessfully;
            m->port = msg->port;
            m->envObjID = 0;
            m->num = 0;

            comModule->sendMessage(m);
        }
        else {
            HubModule::modellingSystem->getEnvObject(msg->envObjID - 1)
                    ->setCoords(msg->coordX, msg->coordY);
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
        // set current robot coords
        messageBump->coordX = object->getCoords().first;
        messageBump->coordY = object->getCoords().second;

        comModule->sendMessage(messageBump);
    }
}

// handle MessageTurn message
void MessageHandler::handle(MessageTurn *msg)
{
    if (msg->envObjID == 0)
        HubModule::modellingSystem->getRobotByPort(msg->port)
                ->setOrientation(msg->degrees);
    else
        HubModule::modellingSystem->getEnvObject(msg->envObjID - 1)
                ->setOrientation(msg->degrees);
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

    for (unsigned int i = 0; i < objVector.size(); i++) {
        MessageObject messageObject;
        // set color
        messageObject.red = objVector.at(i)->getColor().red();
        messageObject.green = objVector.at(i)->getColor().green();
        messageObject.blue = objVector.at(i)->getColor().blue();
        // set coordinates
        messageObject.coordX = objVector.at(i)->getCoords().first;
        messageObject.coordY = objVector.at(i)->getCoords().second;
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
