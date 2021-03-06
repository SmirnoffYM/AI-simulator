#ifndef MESSAGES_H
#define MESSAGES_H

#include <QtCore/QObject>
#include <QtGui/QColor>
#include <QtCore/QLinkedList>
#include <QtCore/QMetaType>
#include <list>

#include "constants.h"

enum MessageType {
    MsgMove = 0,
    MsgTurn,
    MsgChangeSize,
    MsgChangeColor,
    MsgWhoIsThere,
    MsgBump,
    MsgHit,
    MsgMovedSuccessfully,
    MsgThereYouSee,
    MsgNavigationChart,
    MsgParameterReport,
    MsgStart,
    MsgPause,
    MsgUndefined
};

class Message
{
public:
    unsigned int num, port, envObjID;
    MessageType type;

    virtual QDataStream& serialize(QDataStream &);
};

class MessageMove : public Message
{
public:
    MessageMove();
    MessageMove(QDataStream &);

    int coordX, coordY;
};

class MessageTurn : public Message
{
public:
    MessageTurn();
    MessageTurn(QDataStream &);

    int degrees;
};

class MessageChangeSize : public Message
{
public:
    MessageChangeSize();
    MessageChangeSize(QDataStream &);

    unsigned int diameter;
};

class MessageChangeColor : public Message
{
public:
    MessageChangeColor();
    MessageChangeColor(QDataStream &);

    char red, green, blue;
};

class MessageWhoIsThere : public Message
{
public:
    MessageWhoIsThere();
    MessageWhoIsThere(QDataStream &);

    int coordX, coordY;
    unsigned int radius;
};

class MessageBump : public Message
{
public:
    MessageBump();
    QDataStream& serialize(QDataStream &);

    int coordX, coordY;
    char bumpType;
};

class MessageNavigationChart : public Message
{
public:
    MessageNavigationChart();
    QDataStream& serialize(QDataStream &);

    unsigned int fragmentId;
    int coordX, coordY;
    char width, height;
    char* points;
};

class MessageParameterReport : public Message
{
public:
    MessageParameterReport();
    MessageParameterReport(QDataStream &);

    char id;
    int integral;
    unsigned int real;
};

/* Excuse me for awkward name - I really couldn't come up with anything better */
struct MessageObject {
    char type;
    char red, green, blue;
    unsigned int diameter;
    int coordX, coordY;
    int degrees;
};

class MessageThereYouSee : public Message
{
public:
    MessageThereYouSee();
    QDataStream& serialize(QDataStream &);

    unsigned int mapChuncks;
    std::list<MessageObject> objects;
};

#endif // MESSAGES_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
