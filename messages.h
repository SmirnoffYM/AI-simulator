#ifndef MESSAGES_H
#define MESSAGES_H

#include <QtCore/QObject>
#include <QtGui/QColor>
#include <QtCore/QLinkedList>
#include <QtCore/QMetaType>
#include <list>

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

    MessageType getMessageType() const;

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

    Q_PROPERTY(double degrees READ degrees WRITE setDegrees)

public:
    MessageTurn(QObject *parent = 0) : Message(parent) { m_type = "turn"; };

    double degrees() const { return m_degrees; };
    void setDegrees(const double degrees) { m_degrees = degrees; };

private:
    double m_degrees;
};

class MessageChangeSize : public Message
{
    Q_OBJECT

    Q_PROPERTY(int newDiameter READ newDiameter WRITE setNewDiameter)

public:
    MessageChangeSize(QObject *parent = 0) : Message(parent) { m_type = "change size"; };

    int newDiameter() const { return m_newDiameter; };
    void setNewDiameter(const int x) { m_newDiameter = x; };

private:
    int m_newDiameter;
};

class MessageChangeColor : public Message
{
    Q_OBJECT

    Q_PROPERTY(QString newColor READ newColor WRITE setNewColor)

public:
    MessageChangeColor(QObject *parent = 0) : Message(parent) { m_type = "change color"; };

    QString newColor() const { return m_newColor; };
    void setNewColor(const QString newColor)
    {
        m_newColor = newColor;
        m_QColor.setNamedColor(m_newColor);
    };

    QColor getQColor() const { return m_QColor; };
    void setQColor(const QColor c)
    {
        m_QColor = c;
        m_newColor = m_QColor.name();
    };

private:
    QString m_newColor;
    QColor m_QColor;
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

    Q_PROPERTY(QString color READ color WRITE setColor)
    Q_PROPERTY(int diameter READ diameter WRITE setDiameter)
    Q_PROPERTY(qreal orientation READ orientation WRITE setOrientation)
    Q_PROPERTY(int coordX READ coordX WRITE setCoordX)
    Q_PROPERTY(int coordY READ coordY WRITE setCoordY)

public:
    MessageObject(QObject *parent = 0) : QObject(parent) {};
    MessageObject(const MessageObject & other) : QObject()
    {
        m_color = other.color();
        m_diameter = other.diameter();
        m_coordX = other.coordX();
        m_coordY = other.coordY();
        m_orientation = other.orientation();
    };

    QString color() const { return m_color; };
    void setColor(const QString color)
    {
        m_color = color;
        m_qcolor.setNamedColor(m_color);
    };

    int diameter() const { return m_diameter; };
    void setDiameter(const int x) { m_diameter = x; };

    qreal orientation() const { return m_orientation; };
    void setOrientation(const qreal orientation) { m_orientation = orientation; };

    int coordX() const { return m_coordX; };
    void setCoordX(const int x) { m_coordX = x; };

    int coordY() const { return m_coordY; };
    void setCoordY(const int y) { m_coordY = y; };

    QColor getQColor() const { return m_qcolor; };
    void setQColor(const QColor c)
    {
        m_qcolor = c;
        m_color = m_qcolor.name();
    };

private:
    QString m_color;
    QColor m_qcolor;
    int m_diameter, m_coordX, m_coordY;
    qreal m_orientation;
};

class MessageThereYouSee : public Message
{
    Q_OBJECT

    Q_PROPERTY(std::list<MessageObject *> objects READ objects WRITE setObjects)

public:
    MessageThereYouSee(QObject *parent = 0) : Message(parent) { m_type = "there you see"; }

    std::list<MessageObject *> objects() const { return m_objects; }
    void setObjects(const std::list<MessageObject *> objects) { m_objects = objects; }

private:
    std::list<MessageObject *> m_objects;
};

#endif // MESSAGES_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
