#include "message-parsers.h"

MessageMove * parseMessageMove(QDataStream & stream)
{
    MessageMove *msg = new MessageMove();

    quint32 x, y;
    stream >> x >> y;

    msg->coordX = x;
    msg->coordY = y;

    return msg;
};

MessageTurn * parseMessageTurn(QDataStream & stream)
{
    MessageTurn *msg = new MessageTurn();

    qint32 seconds;
    stream >> seconds;

    // 60 seconds is a minute, and 60 minutes is a degree
    msg->degrees = static_cast<double>(seconds) / 3600;

    return msg;
};

MessageChangeSize * parseMessageChangeSize(QDataStream & stream)
{
    MessageChangeSize *msg = new MessageChangeSize();

    quint32 diameter;
    stream >> diameter;

    msg->diameter = diameter;

    return msg;
};

MessageChangeColor * parseMessageChangeColor(QDataStream & stream)
{
    return NULL;
};

MessageWhoIsThere * parseMessageWhoIsThere(QDataStream & stream)
{
    return NULL;
};

MessageParameterReport * parseMessageParameterReport(QDataStream & stream)
{
    return NULL;
};

