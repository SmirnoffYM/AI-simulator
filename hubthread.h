#ifndef HUBTHREAD_H
#define HUBTHREAD_H

#include <QThread>
#include "hubmodule.h"

class HubThread : public QThread
{
Q_OBJECT
public:
    HubThread();
    ~HubThread()
    {
        delete hub;
    }

    void run();
    void stop();

public slots:
    void refresh();

private:
    volatile bool stopped;

    HubModule *hub;

};

#endif // HUBTHREAD_H
