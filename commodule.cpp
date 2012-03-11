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

        // TODO: handle datagram
    }
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
