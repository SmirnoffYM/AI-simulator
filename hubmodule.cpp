#include "hubmodule.h"
#include "servant.h"
#include <cmath>

ModellingSystem * HubModule::modellingSystem = NULL;
double* HubModule::idleTime = NULL;

HubModule::HubModule()
{
    comModule = new ComModule(&messageQueue);

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

            Robot* tmpClientRobot = HubModule::modellingSystem
                    ->getRobotByPort(messageMove->port);

            // check for collisions with robots
            for (int i = 0; i < ROBOTS; i++) {
                Robot* tmpRobot = HubModule::modellingSystem->getRobot(i);
                // if the current robot is the sender robot
                // then make the next loop
                if (tmpRobot->getPortNumber() == messageMove->port)
                    continue;

                // check if distance between two points is
                // bigger then robots size sum
                if (sqrt(
                            pow(messageMove->coordX
                                - tmpRobot->getCoords().first, 2)
                            +
                            pow(messageMove->coordY
                                - tmpRobot->getCoords().second, 2)
                            ) < (tmpRobot->getSize() + tmpClientRobot->getSize())
                        )
                    collision = true;
            }
            // check for collisions with env objects
            for (int i = 0; i < ENV_OBJECTS; i++) {
                EnvObject* tmpEnvObject = HubModule::modellingSystem->getEnvObject(i);
                // check if distance between two points is
                // bigger then robots size sum
                if (tmpEnvObject != NULL && sqrt(
                            pow(messageMove->coordX
                                - tmpEnvObject->getCoords().first, 2)
                            +
                            pow(messageMove->coordY
                                - tmpEnvObject->getCoords().second, 2)
                            ) < (tmpEnvObject->getSize() + tmpClientRobot->getSize())
                        )
                    collision = true;
            }

            if (!collision)
                HubModule::modellingSystem->getRobotByPort(messageMove->port)
                        ->setCoords(messageMove->coordX, messageMove->coordY);
            else {
                MessageBump *messageBump = new MessageBump();
                messageBump->port = messageMove->port;
                // set current robot coords
                messageBump->coordX = tmpClientRobot->getCoords().first;
                messageBump->coordY = tmpClientRobot->getCoords().second;
                comModule->sendMessage(messageBump);
            }
        }
            break;

        case MsgTurn:
        {
            MessageTurn *messageTurn = static_cast<MessageTurn *>(m);

            double prevOrientation = HubModule::modellingSystem->getRobotByPort(messageTurn->port)
                    ->getOrientation();

            HubModule::modellingSystem->getRobotByPort(messageTurn->port)
                    ->setOrientation(prevOrientation + messageTurn->degrees);
        }
            break;

        case MsgChangeSize:
        {
            MessageChangeSize *messageChangeSize = static_cast<MessageChangeSize *>(m);
            HubModule::modellingSystem->getRobotByPort(messageChangeSize->port)
                    ->setSize(messageChangeSize->diameter);
        }
            break;

        case MsgChangeColor:
        {
            MessageChangeColor *messageChangeColor = static_cast<MessageChangeColor *>(m);
            HubModule::modellingSystem->getRobotByPort(messageChangeColor->port)
                    ->setColor(Color(messageChangeColor->red,
                                     messageChangeColor->green,
                                     messageChangeColor->blue));
        }
            break;

        case MsgWhoIsThere:
        {
            // I feel a tremor in the Force.
            // Something is wrong down there

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
                messageObject.coordX = objVector.at(i)->getCoords().second;
                // set diameter
                messageObject.diameter = objVector.at(i)->getSize();
                // set orientation
                messageObject.degrees = objVector.at(i)->getOrientation();

                objectsInRange.push_front(messageObject);
            }

            objVector.clear();

            messageThereYouSee->objects = objectsInRange;
            messageThereYouSee->port = messageWhoIsThere->port;
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
        if (HubModule::idleTime[i] != 0 && HubModule::idleTime[i] <= ROBOT_TIMEOUT)
            HubModule::idleTime[i] += HUB_REFRESH_TIME;
        else if (HubModule::idleTime[i] == 0)
            HubModule::idleTime[i] = START_IDLE_TIME;

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
    case Stopped:
        message->type = MsgStop;
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
