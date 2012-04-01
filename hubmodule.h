#ifndef HUBMODULE_H
#define HUBMODULE_H

#include <QProcess>
#include <queue>

#include "constants.h"
#include "modellingsystem.h"
#include "messages.h"
#include "commodule.h"

class HubModule
{
public:
    HubModule();
    ~HubModule() {}

    static ModellingSystem *modellingSystem;

    void refresh();

    static void LaunchApplications();
    static void StopApplications();
    static void AddApplication(QString command);
private:
    ComModule *comModule;
    std::queue<Message *> messageQueue;
    static std::vector<std::pair<QString, QProcess *> > applications;
};

#endif // HUBMODULE_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
