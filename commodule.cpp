#include "commodule.h"

ComModule::ComModule(QQueue<Message *> *q) : QObject()
{
    messageQueue = q;

    socket = new QUdpSocket(this);
    // FIXME: use the port specified in config
    socket->bind(QHostAddress::LocalHost, SIMULATOR_PORT);

    connect(socket, SIGNAL(readyRead()), this, SLOT(handleMessage()));
}

ComModule::~ComModule()
{
    socket->~QUdpSocket();
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
        
        Message *msg = new Message();
        // FIXME: QQueue is not thread-safe, we need a mutex here once com module is moved to
        // separate thread
        // messageQueue->enqueue(msg);
    }
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
