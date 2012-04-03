#ifndef HUBMODULE_H
#define HUBMODULE_H

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

    static double* getIdleTime();

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
