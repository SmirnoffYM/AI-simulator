#include "commodule.h"

ComModule::ComModule(QQueue<Message *> *q) : QObject()
{
    messageQueue = q;

    socket = new QUdpSocket(this);
    // FIXME: use the port specified in config
    socket->bind(QHostAddress::Any, 9000);

    connect(socket, SIGNAL(readyRead()), this, SLOT(handleMessage()));
}

void ComModule::handleMessage()
{
    while(socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socket->readDatagram(datagram.data(), datagram.size(),
                        &sender, &senderPort);
        
        QJson::Parser parser;
        QVariantMap message = parser.parse(datagram).toMap();

        // parse JSON and put the message into queue
        QString type = message["type"].toString();
        Message *msg = NULL;
        if(type == "move") {
            msg = new MessageMove();
        } else if (type == "turn") {
            msg = new MessageTurn();
        } else if (type == "change size") {
            msg = new MessageChangeSize();
        } else if (type == "change color") {
            msg = new MessageChangeColor();
        } else if (type == "who is there?") {
            msg = new MessageWhoIsThere();
        }
        if(msg != NULL) {
            QJson::QObjectHelper::qvariant2qobject(message, msg);
            messageQueue->enqueue(msg);
        } else {
            // FIXME: stick some QDebug in here
        }
    }
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
