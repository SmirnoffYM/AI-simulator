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
    MessageChangeColor *msg = new MessageChangeColor();

    quint8 red, green, blue;
    stream >> red >> green >> blue;

    msg->red = red;
    msg->green = green;
    msg->blue = blue;

    return msg;
};

MessageWhoIsThere * parseMessageWhoIsThere(QDataStream & stream)
{
    MessageWhoIsThere *msg = new MessageWhoIsThere();

    quint32 x, y, r;
    stream >> x >> y >> r;

    msg->coordX = x;
    msg->coordY = y;
    msg->radius = r;

    return msg;
};

MessageParameterReport * parseMessageParameterReport(QDataStream & stream)
{
    return NULL;
};

