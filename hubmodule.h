#ifndef HUBMODULE_H
#define HUBMODULE_H

#include <QTimer>
#include <queue>
#include <vector>
#include "constants.h"
#include "modellingsystem.h"
#include "messages.h"
#include "commodule.h"

class HubModule : public QObject
{
    Q_OBJECT

public:
    HubModule();
    ~HubModule()
    {
        delete comModule;
    }

    static ModellingSystem *modellingSystem;

    static double* getIdleTime();
    void sendModellingStateMessage(ModellingState state);

public slots:
    void refresh();

private:
    ComModule *comModule;
    std::queue<Message *> messageQueue;

    // Shows how much time each robot was passive
    static double* idleTime;
};

#endif // HUBMODULE_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
