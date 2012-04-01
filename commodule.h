#ifndef COMMODULE_H
#define COMMODULE_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include <queue>

#include <qjson/parser.h>
#include <qjson/qobjecthelper.h>


#include "constants.h"
#include "messages.h"
#include "messages-helpers.h"

class ComModule : public QObject
{
    Q_OBJECT

public:
    ComModule(std::queue<Message *> *);
    ~ComModule();

private slots:
    void handleMessage();

private:
    QUdpSocket *socket;
    std::queue<Message *> *messageQueue;
};

#endif // COMMODULE_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
