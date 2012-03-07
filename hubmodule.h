#ifndef HUBMODULE_H
#define HUBMODULE_H

#include "constants.h"
#include "modellingsystem.h"

class HubModule
{
public:
    HubModule();

    static ModellingSystem *modellingSystem;
};

#endif // HUBMODULE_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
