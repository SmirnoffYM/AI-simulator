#ifndef COMMODULE_H
#define COMMODULE_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include <queue>

#include "constants.h"
#include "messages.h"
#include "message-parsers.h"

class ComModule : public QObject
{
    Q_OBJECT

public:
    /* Constructor receives a pointer to the queue of messages where he would put everything he
     * receives */
    ComModule(std::queue<Message *> *);
    ~ComModule();

    /* This function is getting called from hub module when it's time to send a response */
    void sendMessage(Message *);

private slots:
    /* This slot is getting called when there's UDP messages waiting to be read */
    void handleMessage();

private:
    QUdpSocket *socket;
    std::queue<Message *> *messageQueue;
};

#endif // COMMODULE_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
