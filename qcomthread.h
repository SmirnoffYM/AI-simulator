#ifndef QCOMTHREAD_H
#define QCOMTHREAD_H

#include <QThread>
#include "commodule.h"

class QComThread : public QThread
{
public:
    QComThread();

private:
    ComModule* com;
};

#endif // QCOMTHREAD_H
