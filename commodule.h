#ifndef COMMODULE_H
#define COMMODULE_H

#include <QtCore/QObject>
#include <QtCore/QQueue>
#include <QtCore/QVariant>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>

#include <qjson/parser.h>
#include <qjson/qobjecthelper.h>

#include "constants.h"
#include "messages.h"
#include "message-parsers.h"

class ComModule : public QObject
{
    Q_OBJECT

public:
    ComModule(QQueue<Message *> *);
    ~ComModule();

    void sendMessageToRobot(int portNumber, Message *message);

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
