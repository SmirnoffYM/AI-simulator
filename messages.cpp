#include "messages.h"

QDataStream& serializeMessage(Message *msg, QDataStream &stream) {
    switch(msg->type) {
    case MsgBump:
        (static_cast<MessageBump *>(msg))->serialize(stream);
        break;
    case MsgThereYouSee:
        static_cast<MessageThereYouSee *>(msg)->serialize(stream);
        break;
    case MsgNavigationChart:
        static_cast<MessageNavigationChart *>(msg)->serialize(stream);
        break;
    case MsgMovedSuccessfully:
    case MsgHit:
    case MsgStart:
    case MsgPause:
        // all these messages don't have any payload so it's sufficient to serialize header only
        msg->serialize(stream);
        break;
    default:
        // those messages can't be sent from simulator, thus we don't need serialization code for
        // them
        break;
    }

    return stream;
};

/**** Message
 ******************************************/
QDataStream& Message::serialize(QDataStream &stream) {
    stream << static_cast<quint8>(PROTOCOL_VERSION);
    // other header infortmation
    stream << static_cast<quint32>(num)
           << static_cast<quint8>(envObjID)
           << static_cast<quint16>(port)
           << static_cast<quint8>(type);

    return stream;
}

/**** MessageMove
 ******************************************/
MessageMove::MessageMove() { type = MsgMove; }

MessageMove::MessageMove(QDataStream &stream) {
    // I would have wrote that in C++11 from the beginning if I knew there was delegating
    // constructors in the upcoming standard
    type = MsgMove;

    // it's safe to read directly into unsigned int without casting to quint32 - the latter is
    // just a typedef aliasing to the former
    stream >> coordX >> coordY;
}

/**** MessageTurn
 ******************************************/
MessageTurn::MessageTurn() { type = MsgTurn; }

MessageTurn::MessageTurn(QDataStream &stream) {
    type = MsgTurn;

    qint32 seconds;
    stream >> seconds;

    // 60 seconds is a minute, and 60 minutes is a degree
    degrees = static_cast<double>(seconds) / 3600;
}

/**** MessageChangeSize
 ******************************************/
MessageChangeSize::MessageChangeSize() { type = MsgChangeSize; }

MessageChangeSize::MessageChangeSize(QDataStream &stream) {
    type = MsgChangeSize;

    stream >> diameter;
}

/**** MessageChangeColor
 ******************************************/
MessageChangeColor::MessageChangeColor() { type = MsgChangeColor; }

MessageChangeColor::MessageChangeColor(QDataStream &stream) {
    type = MsgChangeColor;

    // have to use intermediate variables as there's no operator>>(char) defined for QDataStream
    quint8 r, g, b;
    stream >> r >> g >> b;

    red = r;
    green = g;
    blue = b;
}

/**** MessageWhoIsThere
 ******************************************/
MessageWhoIsThere::MessageWhoIsThere() { type = MsgWhoIsThere; }

MessageWhoIsThere::MessageWhoIsThere(QDataStream &stream) {
    type = MsgWhoIsThere;

    stream >> coordX >> coordY >> radius;
}

/**** MessageBump
 ******************************************/
MessageBump::MessageBump() { type = MsgBump; }

QDataStream& MessageBump::serialize(QDataStream &stream) {
    Message::serialize(stream);

    stream << static_cast<quint32>(coordX)
           << static_cast<quint32>(coordY);

    return stream;
}

/**** MessageNavigationChart
 ******************************************/
MessageNavigationChart::MessageNavigationChart() {
    type = MsgNavigationChart;
}

QDataStream& MessageNavigationChart::serialize(QDataStream &stream) {
    Message::serialize(stream);

    stream << static_cast<quint16>(fragmentId)
           << static_cast<qint32>(coordX)
           << static_cast<qint32>(coordY)
           << static_cast<quint8>(width)
           << static_cast<quint8>(height);

    for(char i = 0; i < height; i++) {
        for(char j = 0; j < width; j++) {
            // going through `points' left to right, top to bottom
            stream << static_cast<qint8>(points[i * width + j]);
        }
    }
    
    return stream;
}

/**** MessageParameterReport
 ******************************************/
MessageParameterReport::MessageParameterReport() { type = MsgParameterReport; }

MessageParameterReport::MessageParameterReport(QDataStream &stream) {
    type = MsgParameterReport;

    quint8 i; // id
    stream >> i
           >> integral >> real;

    id = i;
}

/**** MessageThereYouSee
 ******************************************/
MessageThereYouSee::MessageThereYouSee() { type = MsgThereYouSee; }

QDataStream& MessageThereYouSee::serialize(QDataStream &stream) {
    Message::serialize(stream);

    quint32 count = static_cast<quint32>(objects.size());
    stream << count;
    /* For each object, put its description into the stream */
    for(quint32 i = 0; i < count; i++) {
        MessageObject o = objects.front();
        objects.pop_front();
        stream << static_cast<quint32>(o.coordX) << static_cast<quint32>(o.coordY)
               << static_cast<quint32>(o.diameter) << static_cast<quint32>(o.degrees / 3600)
               << static_cast<quint8>(o.red) << static_cast<quint8>(o.green)
               << static_cast<quint8>(o.blue);
    }

    return stream;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
