#include "messages.h"

// ======== Message ========

Message::Message(QObject *parent) : QObject(parent)
{
}

int Message::id() const
{
    return m_id;
}

void Message::setID(const int id)
{
    m_id = id;
}

QString Message::type() const
{
    return m_type;
}

void Message::setType(QString type)
{
    m_type = type;
}

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
