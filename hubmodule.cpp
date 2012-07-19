#include <cmath>
#include "hubmodule.h"
#include "servant.h"

ModellingSystem * HubModule::modellingSystem = NULL;
double* HubModule::idleTime = NULL;

HubModule::HubModule() {
    comModuleThread = new QThread();
    comModule = new ComModule(&messageQueue);
    comModule->moveToThread(comModuleThread);
    comModuleThread->start();

    HubModule::idleTime = new double[ROBOTS];
    for (int i = 0; i < ROBOTS; i++)
        HubModule::idleTime[i] = START_IDLE_TIME;

}
/*
    refreshing system state
    called from hub thread
*/
void HubModule::refresh()
{
    // check if modelling state changed
    // if yes, change
    // current modelling state and send messages to all robots

    if (HubModule::modellingSystem->isModellingStateChanged) {
        HubModule::modellingSystem->isModellingStateChanged = false;
        sendModellingStateMessage(HubModule::modellingSystem->modellingState);
    }


    Message *m = NULL;

    while (!messageQueue.empty())
    {
        m = messageQueue.front();
        messageQueue.pop();

        // set idle time for current robot equal 0
        HubModule::idleTime[HubModule::modellingSystem->getSerialByPortNumber(m->port)] = 0;

        switch (m->type) {
        case MsgMove:
        {
            MessageMove *messageMove = static_cast<MessageMove *>(m);

            // check if there'll be no collisions
            bool collision = false;

            Object* object = NULL;

            if (messageMove->envObjID == 0)
                object = HubModule::modellingSystem
                        ->getRobotByPort(messageMove->port);
            else
                object = HubModule::modellingSystem
                        ->getEnvObject(messageMove->envObjID - 1);


            // check for collisions with robots
            MessageBump *messageBump = new MessageBump();

            for (int i = 0; i < ROBOTS; i++) {
                Robot* tmpRobot = HubModule::modellingSystem->getRobot(i);
                // if the current robot is the sender robot
                // then make the next loop
                if (tmpRobot != NULL
                        && tmpRobot->getPortNumber() == messageMove->port)
                    continue;

                // check if distance between two points is
                // bigger then robots size sum
                if (tmpRobot != NULL
                        && tmpRobot->getSize() != 0
                        && sqrt(
                            pow((int)(messageMove->coordX
                                      - tmpRobot->getCoords().first), 2)
                            +
                            pow((int)(messageMove->coordY
                                      - tmpRobot->getCoords().second), 2)
                            ) < (tmpRobot->getSize() / 2 + object->getSize() / 2)
                        ) {
                    collision = true;
                    // send message to collided robot
                    messageBump->port = tmpRobot->getPortNumber();
                    // just dummy
                    messageBump->num = 0;
                    // why 0? read envObjID specification
                    messageBump->envObjID = 0;
                    // set collided robot coords
                    messageBump->coordX = tmpRobot->getCoords().first;
                    messageBump->coordY = tmpRobot->getCoords().second;
                    comModule->sendMessage(messageBump);
                }
            }
            // check for collisions with env objects
            for (int i = 0; i < ENV_OBJECTS; i++) {
                EnvObject* tmpEnvObject = HubModule::modellingSystem->getEnvObject(i);
                // if the current env object is the sender env object
                // then make the next loop
                if (tmpEnvObject != NULL
                        && tmpEnvObject->getObjectId() == (messageMove->envObjID - 1))
                    continue;
                // check if distance between two points is
                // bigger then robots size sum
                if (tmpEnvObject != NULL
                        && tmpEnvObject->getSize() != 0
                        && sqrt(
                            pow((int)(messageMove->coordX
                                      - tmpEnvObject->getCoords().first), 2)
                            +
                            pow((int)(messageMove->coordY
                                      - tmpEnvObject->getCoords().second), 2)
                            ) < (tmpEnvObject->getSize() / 2 + object->getSize() / 2)
                        ) {
                    collision = true;
                    // send message to collided env object
                    messageBump->port = tmpEnvObject->getPortNumber();
                    // just dummy
                    messageBump->num = 0;
                    // why +1? read envObjID specification
                    messageBump->envObjID = i + 1;
                    // set collided env object coords
                    messageBump->coordX = tmpEnvObject->getCoords().first;
                    messageBump->coordY = tmpEnvObject->getCoords().second;
                    comModule->sendMessage(messageBump);
                }
            }

            if (!collision)
                if (messageMove->envObjID == 0)
                    HubModule::modellingSystem->getRobotByPort(messageMove->port)
                            ->setCoords(messageMove->coordX, messageMove->coordY);
                else
                    HubModule::modellingSystem->getEnvObject(messageMove->envObjID - 1)
                            ->setCoords(messageMove->coordX, messageMove->coordY);
            else {
                messageBump->port = messageMove->port;
                messageBump->envObjID = messageMove->envObjID;
                messageBump->num = messageMove->num;
                // set current robot coords
                messageBump->coordX = object->getCoords().first;
                messageBump->coordY = object->getCoords().second;

                comModule->sendMessage(messageBump);
            }
        }
            break;

        case MsgTurn:
        {
            MessageTurn *messageTurn = static_cast<MessageTurn *>(m);

            if (messageTurn->envObjID == 0)
                HubModule::modellingSystem->getRobotByPort(messageTurn->port)
                        ->setOrientation(messageTurn->degrees);
            else
                HubModule::modellingSystem->getEnvObject(messageTurn->envObjID - 1)
                        ->setOrientation(messageTurn->degrees);
        }
            break;

        case MsgChangeSize:
        {
            MessageChangeSize *messageChangeSize = static_cast<MessageChangeSize *>(m);
            if (messageChangeSize->envObjID == 0)
                HubModule::modellingSystem->getRobotByPort(messageChangeSize->port)
                        ->setSize(messageChangeSize->diameter);
            else
                HubModule::modellingSystem->getEnvObject(messageChangeSize->envObjID - 1)
                        ->setSize(messageChangeSize->diameter);
        }
            break;

        case MsgChangeColor:
        {
            MessageChangeColor *messageChangeColor = static_cast<MessageChangeColor *>(m);
            if (messageChangeColor->envObjID == 0)
                HubModule::modellingSystem->getRobotByPort(messageChangeColor->port)
                        ->setColor(Color(messageChangeColor->red,
                                         messageChangeColor->green,
                                         messageChangeColor->blue));
            else
                HubModule::modellingSystem->getEnvObject(messageChangeColor->envObjID - 1)
                        ->setColor(Color(messageChangeColor->red,
                                         messageChangeColor->green,
                                         messageChangeColor->blue));
        }
            break;

        case MsgWhoIsThere:
        {
            MessageWhoIsThere *messageWhoIsThere = static_cast<MessageWhoIsThere *>(m);

            // we also need to send message about objects in
            // received range, so we must create MessageThereYouSee message
            MessageThereYouSee *messageThereYouSee = new MessageThereYouSee();

            // we also need a list of those objects
            std::list<MessageObject> objectsInRange;


            Robot *robot = HubModule::modellingSystem->getRobotByPort(messageWhoIsThere->port);

            std::vector<Object *> objVector = std::vector<Object *>();
            objVector = robot->iCanSee();

            for (int i = 0; i < objVector.size(); i++) {

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
            messageThereYouSee->port = messageWhoIsThere->port;
            messageThereYouSee->num = messageWhoIsThere->num;
            // send message to robot
            comModule->sendMessage(messageThereYouSee);
        }
            break;

        case MsgParameterReport:
        {
            MessageParameterReport *messageParameterReport
                    = static_cast<MessageParameterReport *>(m);

            double paramValue = messageParameterReport->integral + messageParameterReport->real
                    / 1000000;

            HubModule::modellingSystem->getRobotByPort(messageParameterReport->port)
                    ->setParametersByID(messageParameterReport->id, paramValue);
        }
            break;

        }
    }

    // add time between hub refresh to those robots
    // which were idle in current refresh cycle
    for (int i = 0; i < ROBOTS; i++)
        if (HubModule::idleTime[i] != 0 && HubModule::idleTime[i] <= ROBOT_TIMEOUT
                && HubModule::modellingSystem->modellingState == Started)
            HubModule::idleTime[i] += HUB_REFRESH_TIME;

    QTimer::singleShot(HUB_REFRESH_TIME, this, SLOT(refresh()));
}

double* HubModule::getIdleTime()
{
    return HubModule::idleTime;
}

void HubModule::sendModellingStateMessage(ModellingState state)
{
    Message *message = new Message();
    message->num = 0;

    switch(state) {
    case Started:
        message->type = MsgStart;
        break;
    case Paused:
        message->type = MsgPause;
        break;
    }
    // send message about modelling state
    // to all robots
    for (int i = 0; i < ROBOTS; i++) {
        message->port = HubModule::modellingSystem->getPortBySerialNumber(i);
        comModule->sendMessage(message);
    }

    // send message to env object control program
    message->port = HubModule::modellingSystem->getEnvObjectPortNumber();
    comModule->sendMessage(message);
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
