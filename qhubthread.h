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
