#include <cmath>
#include "hubmodule.h"
#include "servant.h"

ModellingSystem * HubModule::modellingSystem = NULL;
double* HubModule::idleTime = NULL;

HubModule::HubModule() {
    comModuleThread = new QThread();
    comModule = new ComModule(&messageQueue, &msgQueueLock);
    comModule->moveToThread(comModuleThread);
    comModuleThread->start();

    messageHandler = new MessageHandler(comModule);

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

    msgQueueLock.lock();
    while (!messageQueue.empty())
    {
        m = messageQueue.front();
        messageQueue.pop();
        msgQueueLock.unlock();

        // set idle time for current robot equal 0
        HubModule::idleTime[HubModule::modellingSystem->getSerialByPortNumber(m->port)] = 0;

        // handling message
        messageHandler->handle(m);

        msgQueueLock.lock();
    }
    msgQueueLock.unlock();

    // add time between hub refresh to those robots
    // which were idle in current refresh cycle
    for (int i = 0; i < ROBOTS; i++)
        if (HubModule::idleTime[i] != 0 && HubModule::idleTime[i] <= ROBOT_TIMEOUT
                && HubModule::modellingSystem->modellingState == Started) {
            HubModule::idleTime[i] += HUB_REFRESH_TIME;
        }



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
    message->envObjID = 0;

    switch(state) {
    case Started:
        message->type = MsgStart;
        break;
    case Paused:
        message->type = MsgPause;
        break;
    case Stopped:
        // FIXME: explain why Stopped does not need to be handled
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
