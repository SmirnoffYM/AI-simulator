#ifndef HUBTHREAD_H
#define HUBTHREAD_H

#include <QThread>
#include "hubmodule.h"

class HubThread : public QThread
{
public:
    HubThread();

    void run();
    void stop();

private:
    volatile bool stopped;

    HubModule *hub;

};

#endif // HUBTHREAD_H
