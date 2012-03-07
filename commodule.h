#ifndef COMMODULE_H
#define COMMODULE_H

#include <QtNetwork/QUdpSocket>
#include "constants.h"

class ComModule
{
public:
    ComModule();

private:
    QUdpSocket *socket;
};

#endif // COMMODULE_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
