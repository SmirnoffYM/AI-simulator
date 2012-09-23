#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "messages.h"
#include "commodule.h"

class MessageHandler
{
private:
    ComModule *comModule;

public:

    MessageHandler(ComModule *comModule);

    void handle(Message *msg);
    void handle(MessageMove *msg);
    void handle(MessageTurn *msg);
    void handle(MessageChangeSize *msg);
    void handle(MessageChangeColor *msg);
    void handle(MessageWhoIsThere *msg);
    void handle(MessageParameterReport *msg);

};

#endif // MESSAGEHANDLER_H
