#include "messages.h"

MessageMove parseMessageMove(QDataStream *);
MessageMove parseMessageTurn(QDataStream *);
MessageMove parseMessageChangeSize(QDataStream *);
MessageMove parseMessageChangeColor(QDataStream *);
MessageMove parseMessageWhoIsThere(QDataStream *);
MessageMove parseMessageParameterReport(QDataStream *);
