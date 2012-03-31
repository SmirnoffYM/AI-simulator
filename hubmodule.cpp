#include "hubmodule.h"
#include <QTimer>
#include <QDebug>
#include <iostream>

ModellingSystem * HubModule::modellingSystem = NULL;

HubModule::HubModule(QObject *parent) :
    QObject(parent)
{
    messageQueue = new QQueue<Message *>();
    comModule = new ComModule(messageQueue);
}
/*
    refreshing system state
    called from hub thread
*/
void HubModule::refresh()
{
   if (ModellingSystem::isModellingPerformed) {
        Message *m = NULL;

        while (!messageQueue->isEmpty())
        {
           m = messageQueue->dequeue();

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
               HubModule::modellingSystem->getRobotByPort(messageChangeColor->id())
                       ->setColor(messageChangeColor->newColor());
           }
        }

        QTimer::singleShot(1000 / SCREEN_REFRESH_RATE, this, SLOT(refresh()));
    }
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
