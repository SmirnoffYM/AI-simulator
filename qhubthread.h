#ifndef QHUBTHREAD_H
#define QHUBTHREAD_H

#include <QThread>
#include "hubmodule.h"

class QHubThread : public QThread
{
public:
    QHubThread();

private:
    HubModule* hub;
};

#endif // QHUBTHREAD_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
