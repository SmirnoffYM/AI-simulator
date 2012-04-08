#include "message-parsers.h"

MessageMove * parseMessageMove(QDataStream & stream)
{
    quint32 x, y;
    stream >> x >> y;

    return NULL;
};

MessageTurn * parseMessageTurn(QDataStream & stream)
{
    return NULL;
};

MessageChangeSize * parseMessageChangeSize(QDataStream & stream)
{
    return NULL;
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

