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

        /* QDataStream would handle endianness for us */
        QDataStream stream(&datagram, QIODevice::WriteOnly);

        quint8 version;
        stream >> version;
        if(version != 1) {
            // FIXME: stick qDebug in here
            return;
        }

        quint32 seq_num;
        quint16 port;
        quint8 msg_type;
        stream >> seq_num >> port >> msg_type;
        msg->num = seq_num;
        msg->port = port;
        msg->type = (MessageType)msg_type;

        switch(msg->type) {
        case MsgAcknowledge:
            break;
        case MsgMove:
            break;
        case MsgTurn:
            break;
        case MsgChangeSize:
            break;
        case MsgChangeColor:
            break;
        case MsgWhoIsThere:
            break;
        case MsgBump:
            break;
        case MsgThereYouSee:
            break;
        case MsgParameterReport:
            break;
        default:
            // FIXME: qDebug
            break;
        }

        // FIXME: QQueue is not thread-safe, we need a mutex here once com module is moved to
        // separate thread
        // messageQueue->enqueue(msg);
    }
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
