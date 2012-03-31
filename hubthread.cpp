#include "hubthread.h"
#include <QTimer>

HubThread::HubThread()
{
    stopped = false;
    hub = new HubModule();
}

void HubThread::run()
{
    QTimer::singleShot(0, hub, SLOT(refresh()));
}

void HubThread::stop()
{
    stopped = true;
}
