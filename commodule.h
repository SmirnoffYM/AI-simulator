#ifndef COMMODULE_H
#define COMMODULE_H

#include <QtCore/QObject>
#include <QtCore/QQueue>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>

#include "constants.h"
#include "messages.h"

class ComModule : public QObject
{
    Q_OBJECT

public:
    ComModule(QQueue<Message *> *);

private slots:
    void handleMessage();

private:
    QUdpSocket *socket;
    QQueue<Message *> *messageQueue;
};

#endif // COMMODULE_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
