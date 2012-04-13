#include "commodule.h"

ComModule::ComModule(std::queue<Message *> *q) : QObject()
{
    messageQueue = q;

    socket = new QUdpSocket(this);
    // FIXME: use the port specified in config
    socket->bind(QHostAddress::LocalHost, SIMULATOR_PORT);

    /* Call handleMessage() every time there's something to read in socket - i.e. when message is
     * received */
    connect(socket, SIGNAL(readyRead()), this, SLOT(handleMessage()));
}

ComModule::~ComModule()
{
    socket->~QUdpSocket();
}

void ComModule::handleMessage()
{
    /* More messages may appear one we process the first one, so let's wrap the core into the loop
     * which would iterate while there's something in the socket to read */
    while(socket->hasPendingDatagrams()) {
        /* We represent message as an array of bytes, for which Qt has a handy class */
        QByteArray datagram;
        /* QByteArray can grow as data is added, but we can speed things up by explicitly asking for
         * the space we would need */
        datagram.resize(socket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;

        /* Read the message */
        socket->readDatagram(datagram.data(), datagram.size(),
                        &sender, &senderPort);
        
        /* The point of all this method is to put message into the queue
         * This variable would point to that message */
        Message *msg = NULL;

        /* QDataStream would handle endianness for us */
        QDataStream stream(&datagram, QIODevice::ReadOnly);

        quint8 version;
        stream >> version;
        if(version != 1) {
            // FIXME: stick qDebug in here
            // VERSION MISMATCH, CAN'T HANDLE THAT
            return;
        }

        /* Read some service info */
        quint32 seq_num;
        quint16 port;
        quint8 msg_type;
        stream >> seq_num >> port >> msg_type;

        /* Dispatch depending on the message type */
        switch(msg_type) {
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
            // Receiving anything else than the messages specified above is kind of an error
            break;
        }

        /* At this point, we have 'msg' pointing at the parsed message represented by some subclass
         * of Message class (see messages.h). All we need to do now is to put service info in and
         * then put the message into the queue */
        msg->num = seq_num;
        msg->port = port;
        msg->type = (MessageType)msg_type;

        // FIXME: is std::queue thread-safe? Should we use mutex here?
        messageQueue->push(msg);
    }
}

void ComModule::sendMessage(Message *msg)
{
    /* datagram would contain the message to send */
    QByteArray datagram;
    /* Use QDataStream to populate datagram with data because it would handle endianness for us */
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
        /* For each object, put its description into the stream */
        for(quint32 i = 0; i < count; i++) {
            MessageObject o = m->objects.front();
            m->objects.pop_front();
            stream << (quint32)o.coordX << (quint32)o.coordY
                   << (quint32)o.diameter << (quint32)(o.degrees / 3600)
                   << (quint8)o.red << (quint8)o.green << (quint8)o.blue;
        }
        break;
    }

    /* Send the message */
    socket->writeDatagram(datagram, QHostAddress::LocalHost, msg->port);
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
