#include "messages.h"
#include "messages-helpers.h"

MessageType Message::getMessageType() const
{
    return stringToMessageType(m_type);
};
