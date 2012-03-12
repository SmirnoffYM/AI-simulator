#include "hubmodule.h"

ModellingSystem * HubModule::modellingSystem = NULL;

HubModule::HubModule()
{
    messageQueue = new QQueue<Message *>();
    comModule = new ComModule(messageQueue);
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
