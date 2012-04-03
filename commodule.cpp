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
        
        QJson::Parser parser;
        QVariantMap message = parser.parse(datagram).toMap();

        // parse JSON and put the message into queue
        MessageType type = stringToMessageType(message["type"].toString());
        Message *msg = NULL;
        switch(type) {
            case MsgMove:
                msg = new MessageMove();
                break;

            case MsgTurn:
                msg = new MessageTurn();
                break;

            case MsgChangeSize:
                msg = new MessageChangeSize();
                break;

            case MsgChangeColor:
                msg = new MessageChangeColor();
                break;

            case MsgWhoIsThere:
                msg = new MessageWhoIsThere();
                break;
            
            default:
                // TODO: insert QDebug here?
                break;
        }
        if(msg != NULL) {
            QJson::QObjectHelper::qvariant2qobject(message, msg);
            // FIXME: QQueue is not thread-safe, we need a mutex here once com module is moved to
            // separate thread
            messageQueue->enqueue(msg);
        } else {
            // FIXME: stick some QDebug in here
        }
    }
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
