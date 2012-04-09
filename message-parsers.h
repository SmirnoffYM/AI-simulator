#include "messages.h"

MessageMove * parseMessageMove(QDataStream &);
MessageTurn * parseMessageTurn(QDataStream &);
MessageChangeSize * parseMessageChangeSize(QDataStream &);
MessageChangeColor * parseMessageChangeColor(QDataStream &);
MessageWhoIsThere * parseMessageWhoIsThere(QDataStream &);
MessageParameterReport * parseMessageParameterReport(QDataStream &);
