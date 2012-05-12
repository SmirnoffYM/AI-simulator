#include "hubthread.h"
#include <QTimer>

HubThread::HubThread()
{
    stopped = false;
    hub = new HubModule();
}

void HubThread::run()
{
    QTimer::singleShot(0, this, SLOT(refresh()));
}

void HubThread::stop()
{
    stopped = true;
}

void HubThread::refresh()
{
    if (ModellingSystem::modellingState == Started) {
        hub->refresh();
        QTimer::singleShot(HUB_REFRESH_TIME, this, SLOT(refresh()));
    }
}
