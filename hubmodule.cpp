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
    Message *m = NULL;

    while (!messageQueue.empty())
    {
        m = messageQueue.front();

        // set idle time for current robot equal 0
        HubModule::idleTime[HubModule::modellingSystem->getSerialByPortNumber(m->id())] = 0;

        if(m->type() == "move")
        {
            MessageMove *messageMove = static_cast<MessageMove *>(m);
            HubModule::modellingSystem->getRobotByPort(messageMove->id())
                    ->setCoords(messageMove->coordX(), messageMove->coordY());

        } else if (m->type() == "turn")
        {
            MessageTurn *messageTurn = static_cast<MessageTurn *>(m);

            double prevOrientation = HubModule::modellingSystem->getRobotByPort(messageTurn->id())
                    ->getOrientation();

            HubModule::modellingSystem->getRobotByPort(messageTurn->id())
                    ->setOrientation(prevOrientation + messageTurn->degrees());

        } else if (m->type() == "change size")
        {
            m = messageQueue.front();

            if(m->type() == "move")
            {
                MessageMove *messageMove = static_cast<MessageMove *>(m);
                HubModule::modellingSystem->getRobotByPort(messageMove->id())
                        ->setCoords(messageMove->coordX(), messageMove->coordY());

            } else if (m->type() == "turn")
            {
                MessageTurn *messageTurn = static_cast<MessageTurn *>(m);

                double prevOrientation = HubModule::modellingSystem->getRobotByPort(messageTurn->id())
                        ->getOrientation();

                HubModule::modellingSystem->getRobotByPort(messageTurn->id())
                        ->setOrientation(prevOrientation + messageTurn->degrees());

            } else if (m->type() == "change size")
            {
                MessageChangeSize *messageChangeSize = static_cast<MessageChangeSize *>(m);
                HubModule::modellingSystem->getRobotByPort(messageChangeSize->id())
                        ->setSize(messageChangeSize->newDiameter());

            } else if (m->type() == "change color")
            {
                MessageChangeColor *messageChangeColor = static_cast<MessageChangeColor *>(m);
                QColor col = QColor(messageChangeColor->newColor());
                HubModule::modellingSystem->getRobotByPort(messageChangeColor->id())
                        ->setColor(Color(col.red(), col.green(), col.blue()));
            }

            messageQueue.pop();

            MessageChangeSize *messageChangeSize = static_cast<MessageChangeSize *>(m);
            HubModule::modellingSystem->getRobotByPort(messageChangeSize->id())
                    ->setSize(messageChangeSize->newDiameter());

        } else if (m->type() == "change color")
        {
            MessageChangeColor *messageChangeColor = static_cast<MessageChangeColor *>(m);
            QColor col = QColor(messageChangeColor->newColor());
            HubModule::modellingSystem->getRobotByPort(messageChangeColor->id())
                    ->setColor(Color(col.red(), col.green(), col.blue()));
        } else if (m->type() == "who is there?")
        {
            // I feel a tremor in the Force.
            // Something is wrong down there

            MessageWhoIsThere *messageWhoIsThere = static_cast<MessageWhoIsThere *>(m);

            // we also need to send message about objects in
            // received range, so we must create MessageThereYouSee message
            MessageThereYouSee *messageThereYouSee = new MessageThereYouSee();

            // we also need a list of those objects
            std::list<MessageObject *> objectsInRange;

            // look through all robots and put into list those,
            // which are in received radius
            for (int i = 0; i < ROBOTS; i++)
            {
                Robot *robot = HubModule::modellingSystem->getRobot(i);

                // check if message parent's id not equal
                // to current 'i' robot
                // and
                // check if distance between them is not bigger then
                // received radius
                if ((robot->getPortNumber() != messageWhoIsThere->id())
                        && (sqrt(
                                pow(robot->getCoords().first
                                    - messageWhoIsThere->coordX(), 2)
                                +
                                pow(robot->getCoords().second
                                    - messageWhoIsThere->coordY(), 2)
                                ) <= messageWhoIsThere->radius()
                            )
                        )
                {
                    MessageObject *messageObject = new MessageObject();
                    messageObject->setColor(QString::fromStdString(robot->getColor().toString()));
                    messageObject->setCoordX(robot->getCoords().first);
                    messageObject->setCoordY(robot->getCoords().second);
                    messageObject->setDiameter(robot->getSize());
                    messageObject->setOrientation(robot->getOrientation());

                    objectsInRange.push_front(messageObject);

                    // FIXME: messageObject destructor here
                }
            }


            // look through all environment objects and put into list those,
            // which are in received radius
            for (int i = 0; i < ENV_OBJECTS; i++)
            {
                EnvObject *envObject = HubModule::modellingSystem->getEnvObject(i);

                // check if message parent's id not equal
                // to current 'i' robot
                // and
                // check if distance between them is not bigger then
                // received radius
                if ((envObject->getPortNumber() != messageWhoIsThere->id())
                        && (sqrt(
                                pow(envObject->getCoords().first
                                    - messageWhoIsThere->coordX(), 2)
                                +
                                pow(envObject->getCoords().second
                                    - messageWhoIsThere->coordY(), 2)
                                ) <= messageWhoIsThere->radius()
                            )
                        )
                {
                    MessageObject *messageObject = new MessageObject();
                    messageObject->setColor(QString::fromStdString(envObject->getColor().
                                                                   toString()));
                    messageObject->setCoordX(envObject->getCoords().first);
                    messageObject->setCoordY(envObject->getCoords().second);
                    messageObject->setDiameter(envObject->getSize());
                    messageObject->setOrientation(envObject->getOrientation());

                    objectsInRange.push_front(messageObject);

                    // FIXME: messageObject destructor here
                }
            }

            messageThereYouSee->setObjects(objectsInRange);
            // send message to robot
            comModule->sendMessageToRobot(messageThereYouSee->id(), messageThereYouSee);
        }

        messageQueue.pop();
    }

    // add time between hub refresh to those robots
    // which were idle in current refresh cycle
    for (int i = 0; i < ROBOTS; i++)
        if (HubModule::idleTime[i] != 0 && HubModule::idleTime[i] <= ROBOT_TIMEOUT)
             HubModule::idleTime[i] += HUB_REFRESH_TIME;

}

double* HubModule::getIdleTime()
{
    return HubModule::idleTime;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
