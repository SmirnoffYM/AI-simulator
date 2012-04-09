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
        
        Message *msg = NULL;

        /* QDataStream would handle endianness for us */
        QDataStream stream(&datagram, QIODevice::ReadOnly);

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

        switch(msg->type) {
        case MsgMove:
            msg = parseMessageMove(stream);
            break;
        case MsgTurn:
            msg = parseMessageTurn(stream);
            break;
        case MsgChangeSize:
            msg = parseMessageChangeSize(stream);
            break;
        case MsgChangeColor:
            msg = parseMessageChangeColor(stream);
            break;
        case MsgWhoIsThere:
            msg = parseMessageWhoIsThere(stream);
            break;
        case MsgParameterReport:
            msg = parseMessageParameterReport(stream);
            break;
        default:
            // FIXME: qDebug
            break;
        }

        msg->num = seq_num;
        msg->port = port;
        msg->type = (MessageType)msg_type;

        // FIXME: QQueue is not thread-safe, we need a mutex here once com module is moved to
        // separate thread
        messageQueue->push(msg);
    }
}

void ComModule::sendMessage(Message *msg)
{
    QByteArray datagram;
    QDataStream stream(&datagram, QIODevice::WriteOnly);

    // version: 1
    stream << (quint8)1;
    // other header infortmation
    stream << (quint32)msg->num << (quint16)msg->port << (quint8)msg->type;

    switch(msg->type) {
    case MsgAcknowledge:
        // "acknowledge" doesn't have any payload
        break;
    case MsgBump:
        stream << (quint32)((MessageBump *)msg)->coordX
               << (quint32)((MessageBump *)msg)->coordY;
        break;
    case MsgThereYouSee:
        MessageThereYouSee *m = (MessageThereYouSee *)msg;
        quint32 count = (quint32)m->objects.size();
        stream << count;
        for(quint32 i = 0; i < count; i++) {
            MessageObject o = m->objects.takeFirst();
            stream << (quint32)o.coordX << (quint32)o.coordY
                   << (quint32)o.diameter << (quint32)o.seconds
                   << (quint8)o.red << (quint8)o.green << (quint8)o.blue;
        }
        break;
    }

    socket->writeDatagram(datagram, QHostAddress::LocalHost, msg->port);
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
