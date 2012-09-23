#ifndef MESSAGES_H
#define MESSAGES_H

#include <QtCore/QObject>
#include <QtGui/QColor>
#include <QtCore/QLinkedList>
#include <QtCore/QMetaType>
#include <list>

#include "messages.h"

enum MessageType {
    MsgMove = 0,
    MsgTurn,
    MsgChangeSize,
    MsgChangeColor,
    MsgWhoIsThere,
    MsgBump,
    MsgThereYouSee,
    MsgParameterReport,
    MsgStart,
    MsgPause,
    MsgUndefined
};

class Message
{
public:
    int num, port, envObjID;
    MessageType type;
};

class MessageMove : public Message
{
public:
    MessageMove() { type = MsgMove; }

    MessageMove(QDataStream & stream) {
        // I would have wrote that in C++11 from the beginning if I knew there was delegating
        // constructors in the upcoming standard
        type = MsgMove;

        // it's safe to read directly into unsigned int without casting to quint32 - the latter is
        // just a typedef aliasing to the former
        stream >> coordX >> coordY;
    }

    unsigned int coordX, coordY;

};

class MessageBump : public Message
{
public:
    MessageBump() { type = MsgBump; }

    unsigned int coordX, coordY;
};

class MessageTurn : public Message
{
public:
    MessageTurn() { type = MsgTurn; }

    MessageTurn(QDataStream & stream) {
        type = MsgTurn;

        qint32 seconds;
        stream >> seconds;

        // 60 seconds is a minute, and 60 minutes is a degree
        degrees = static_cast<double>(seconds) / 3600;
    }

    double degrees; 

};

class MessageChangeSize : public Message
{
public:
    MessageChangeSize() { type = MsgChangeSize; }

    MessageChangeSize(QDataStream & stream) {
        type = MsgChangeSize;

        stream >> diameter;
    }

    unsigned int diameter;
};

class MessageChangeColor : public Message
{
public:
    MessageChangeColor() { type = MsgChangeColor; }

    MessageChangeColor(QDataStream & stream) {
        type = MsgChangeColor;

        // have to use intermediate variables as there's no operator>>(char) defined for QDataStream
        quint8 r, g, b;
        stream >> r >> g >> b;

        red = r;
        green = g;
        blue = b;
    }

    char red, green, blue;
};

class MessageWhoIsThere : public Message
{
public:
    MessageWhoIsThere() { type = MsgWhoIsThere; }

    MessageWhoIsThere(QDataStream & stream) {
        type = MsgWhoIsThere;

        stream >> coordX >> coordY >> radius;
    }

    unsigned int coordX, coordY; 
    unsigned int radius;
};

class MessageParameterReport : public Message
{
public:
    MessageParameterReport() { type = MsgParameterReport; }

    MessageParameterReport(QDataStream & stream) {
        type = MsgParameterReport;

        quint8 i; // id
        stream >> i
               >> integral >> real;

        id = i;
    }
    
    char id;
    int integral;
    unsigned int real;

};

/* Excuse me for awkward name - I really couldn't come up with anything better */
struct MessageObject {
    char red, green, blue;
    unsigned int diameter;
    unsigned int coordX, coordY;
    double degrees;

};

class MessageThereYouSee : public Message
{
public:
    MessageThereYouSee() { type = MsgThereYouSee; }

    std::list<MessageObject> objects;
};

#endif // MESSAGES_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
