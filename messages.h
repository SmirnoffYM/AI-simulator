#ifndef MESSAGES_H
#define MESSAGES_H

#include <QtCore/QObject>
#include <QtGui/QColor>
#include <QtCore/QLinkedList>

enum MessageType {
    MsgMove = 0,
    MsgBump,
    MsgTurn,
    MsgChangeSize,
    MsgChangeColor,
    MsgWhoIsThere,
    MsgThereYouSee,
    MsgUndefined
};

class Message : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setID)
    Q_PROPERTY(QString type READ type WRITE setType)

public:
    Message(QObject *parent = 0) : QObject(parent) {};
    
    int id() const { return m_id; };
    void setID(const int id) { m_id = id; };

    QString type() const { return m_type; };
    void setType(QString type) { m_type = type; };

protected:
    int m_id;
    QString m_type;
};

class MessageMove : public Message
{
    Q_OBJECT

    Q_PROPERTY(int coordX READ coordX WRITE setCoordX)
    Q_PROPERTY(int coordY READ coordY WRITE setCoordY)

public:
    MessageMove(QObject *parent = 0) : Message(parent) { m_type = "move"; };

    int coordX() const { return m_coordX; };
    void setCoordX(const int x) { m_coordX = x; };

    int coordY() const { return m_coordY; };
    void setCoordY(const int y) { m_coordY = y; };

private:
    int m_coordX, m_coordY;
};

/* MessageBump is just like MessageMove */
class MessageBump : public Message
{
    Q_OBJECT

    Q_PROPERTY(int coordX READ coordX WRITE setCoordX)
    Q_PROPERTY(int coordY READ coordY WRITE setCoordY)

public:
    MessageBump(QObject *parent = 0) : Message(parent) { m_type = "bump"; };

    int coordX() const { return m_coordX; };
    void setCoordX(const int x) { m_coordX = x; };

    int coordY() const { return m_coordY; };
    void setCoordY(const int y) { m_coordY = y; };

private:
    int m_coordX, m_coordY;
};

class MessageTurn : public Message
{
    Q_OBJECT

    Q_PROPERTY(qreal degrees READ degrees WRITE setDegrees)

public:
    MessageTurn(QObject *parent = 0) : Message(parent) { m_type = "turn"; };

    qreal degrees() const { return m_degrees; };
    void setDegrees(const qreal degrees) { m_degrees = degrees; };

private:
    qreal m_degrees;
};

class MessageChangeSize : public Message
{
    Q_OBJECT

    Q_PROPERTY(int newWidth READ newWidth WRITE setNewWidth)
    Q_PROPERTY(int newHeight READ newHeight WRITE setNewHeight)

public:
    MessageChangeSize(QObject *parent = 0) : Message(parent) { m_type = "change size"; };

    int newWidth() const { return m_newWidth; };
    void setNewWidth(const int x) { m_newWidth = x; };

    int newHeight() const { return m_newHeight; };
    void setNewHeight(const int y) { m_newHeight = y; };

private:
    int m_newWidth, m_newHeight;
};

class MessageChangeColor : public Message
{
    Q_OBJECT

    Q_PROPERTY(qreal newColor READ newColor WRITE setNewColor)

public:
    MessageChangeColor(QObject *parent = 0) : Message(parent) { m_type = "change color"; };

    qreal newColor() const { return m_newColor; };
    void setNewColor(const qreal newColor) { m_newColor = newColor; };

private:
    qreal m_newColor;
};

class MessageWhoIsThere : public Message
{
    Q_OBJECT

    Q_PROPERTY(int coordX READ coordX WRITE setCoordX)
    Q_PROPERTY(int coordY READ coordY WRITE setCoordY)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius)

public:
    MessageWhoIsThere(QObject *parent = 0) : Message(parent) { m_type = "who is there?"; };

    int coordX() const { return m_coordX; };
    void setCoordX(const int x) { m_coordX = x; };

    int coordY() const { return m_coordY; };
    void setCoordY(const int y) { m_coordY = y; };

    qreal radius() const { return m_radius; };
    void setRadius(const qreal r) { m_radius = r; };

private:
    int m_coordX, m_coordY;
    qreal m_radius;
};

/* Excuse me for awkward name - I really couldn't come up with anything better */
class MessageObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(int width READ width WRITE setWidth)
    Q_PROPERTY(int height READ height WRITE setHeight)
    Q_PROPERTY(qreal orientation READ orientation WRITE setOrientation)
    Q_PROPERTY(int coordX READ coordX WRITE setCoordX)
    Q_PROPERTY(int coordY READ coordY WRITE setCoordY)

public:
    MessageObject(QObject *parent = 0) : QObject(parent) {};
    MessageObject(const MessageObject & other) : QObject()
    {
        m_color = other.color();
        m_width = other.width();
        m_height = other.height();
        m_coordX = other.coordX();
        m_coordY = other.coordY();
        m_orientation = other.orientation();
    };

    QColor color() const { return m_color; };
    void setColor(const QColor color) { m_color = color; };

    int width() const { return m_width; };
    void setWidth(const int x) { m_width = x; };

    int height() const { return m_height; };
    void setHeight(const int y) { m_height = y; };

    qreal orientation() const { return m_orientation; };
    void setOrientation(const qreal orientation) { m_orientation = orientation; };

    int coordX() const { return m_coordX; };
    void setCoordX(const int x) { m_coordX = x; };

    int coordY() const { return m_coordY; };
    void setCoordY(const int y) { m_coordY = y; };

private:
    QColor m_color;
    int m_width, m_height, m_coordX, m_coordY;
    qreal m_orientation;
};

class MessageThereYouSee : public Message
{
    Q_OBJECT

    Q_PROPERTY(QLinkedList<MessageObject> objects READ objects WRITE setObjects)

public:
    MessageThereYouSee(QObject *parent = 0) : Message(parent) { m_type = "there you see"; };

    QLinkedList<MessageObject> objects() const { return m_objects; };
    void setObjects(const QLinkedList<MessageObject> objects) { m_objects = objects; };

private:
    QLinkedList<MessageObject> m_objects;
};

#endif // MESSAGES_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
