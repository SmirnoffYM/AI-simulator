#ifndef HUBMODULE_H
#define HUBMODULE_H

#include <QtCore/QQueue>
#include <QObject>

#include "constants.h"
#include "modellingsystem.h"
#include "messages.h"
#include "commodule.h"

class HubModule : public QObject
{
Q_OBJECT
public:
    HubModule(QObject *parent = 0);
    ~HubModule() {}

    static ModellingSystem *modellingSystem;

public slots:
    void refresh();

private:
    ComModule *comModule;
    QQueue<Message *> *messageQueue;
};

#endif // HUBMODULE_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
