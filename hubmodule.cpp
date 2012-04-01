#include "hubmodule.h"

ModellingSystem * HubModule::modellingSystem = NULL;
std::vector<std::pair<QString, QProcess *> > HubModule::applications =
        std::vector<std::pair<QString, QProcess *> >();

HubModule::HubModule()
{
    comModule = new ComModule(&messageQueue);
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

           messageQueue.pop();
        }

}

void HubModule::AddApplication(QString command)
{
    QProcess *application = new QProcess();
    applications.push_back(std::pair<QString, QProcess*>(command, application));
}

void HubModule::LaunchApplications()
{
    for (int i = 0; i < applications.size(); i++) {
        applications.at(i).second->start(applications.at(i).first);
    }
}

void HubModule::StopApplications()
{
    for (int i = 0; i < applications.size(); i++) {
        applications.at(i).second->kill();
    }
    applications.clear();
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
