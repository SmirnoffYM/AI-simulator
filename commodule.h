#ifndef COMMODULE_H
#define COMMODULE_H

#include "constants.h"
#include <QtNetwork/QUdpSocket>

class ComModule
{
public:
    ComModule();

private:
    QUdpSocket* socket;
};

#endif // COMMODULE_H
