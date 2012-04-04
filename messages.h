#ifndef MESSAGES_H
#define MESSAGES_H

#include <QtCore/QObject>
#include <QtGui/QColor>
#include <QtCore/QLinkedList>
#include <QtCore/QMetaType>

enum MessageType {
    MsgAcknowledge = 0,
    MsgMove,
    MsgTurn,
    MsgChangeSize,
    MsgChangeColor,
    MsgWhoIsThere,
    MsgBump,
    MsgThereYouSee,
    MsgParameterReport,
    MsgUndefined
};

class Message
{
public:
    int num, port;
    MessageType type;
};

class MessageMove : public Message
{
public:
    MessageMove() { type = MsgMove; };

    unsigned int coordX, coordY;
};

class MessageBump : public Message
{
public:
    MessageBump() { type = MsgBump; };

    unsigned int coordX, coordY;
};

class MessageTurn : public Message
{
public:
    MessageTurn() { type = MsgTurn; };

    double degrees; 
};

class MessageChangeSize : public Message
{
public:
    MessageChangeSize() { type = MsgChangeSize; };

    unsigned int diameter; 
};

class MessageChangeColor : public Message
{
public:
    MessageChangeColor() { type = MsgChangeColor; };

    char red, green, blue; 
};

class MessageWhoIsThere : public Message
{
public:
    MessageWhoIsThere() { type = MsgWhoIsThere; };

    unsigned int coordX, coordY; 
    unsigned int radius;
};

/* Excuse me for awkward name - I really couldn't come up with anything better */
struct MessageObject {
    char red, green, blue;
    unsigned int diameter;
    unsigned int coordX, coordY;
    int seconds;
};

class MessageThereYouSee : public Message {
public:
    MessageThereYouSee() { type = MsgThereYouSee; };

    QLinkedList<MessageObject> objects;
};

#endif // MESSAGES_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
