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
    if (ModellingSystem::isModellingPerformed) {
        hub->refresh();
        QTimer::singleShot(1000 / SCREEN_REFRESH_RATE, this, SLOT(refresh()));
    }
}
