#ifndef QCOMTHREAD_H
#define QCOMTHREAD_H

#include <QThread>
#include "commodule.h"

class QComThread : public QThread
{
public:
    QComThread();

    void run();
private:
    ComModule* com;
};

#endif // QCOMTHREAD_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
