#ifndef MESSAGES_H
#define MESSAGES_H

#include <QtCore/QObject>

class Message : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setID)
    Q_PROPERTY(QString type READ type WRITE setType)

public:
    Message(QObject *parent = 0);
    
    int id() const;
    void setID(const int id);

    QString type() const;
    void setType(QString type);

private:
    int m_id;
    QString m_type;
};

#endif // MESSAGES_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
