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
    /* Constructor receives a pointer to the queue of messages where he would put everything he
     * receives */
    ComModule(QQueue<Message *> *);
    ~ComModule();

    /* This function is getting called from hub module when it's time to send a response */
    void sendMessage(Message *);

private slots:
    /* This slot is getting called when there's UDP messages waiting to be read */
    void handleMessage();

private:
    QUdpSocket *socket;
    QQueue<Message *> *messageQueue;
};

#endif // COMMODULE_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
