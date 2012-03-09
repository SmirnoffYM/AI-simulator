#ifndef COMMODULE_H
#define COMMODULE_H

#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include <QtCore/QObject>
#include "constants.h"

class ComModule : public QObject
{
    Q_OBJECT

public:
    ComModule();

private slots:
    void handleMessage();

private:
    QUdpSocket *socket;
};

#endif // COMMODULE_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
