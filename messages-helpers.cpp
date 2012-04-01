#include "messages-helpers.h"

MessageType stringToMessageType(QString type)
{
    if(type == "move") {
        return MsgMove;
    } else if (type == "bump") {
        return MsgBump;
    } else if (type == "turn") {
        return MsgTurn;
    } else if (type == "change size") {
        return MsgChangeSize;
    } else if (type == "change color") {
        return MsgChangeColor;
    } else if (type == "who is there?") {
        return MsgWhoIsThere;
    } else if (type == "there you see") {
        return MsgThereYouSee;
    } else {
        // TODO: insert QDebug here?
        return MsgUndefined;
    }
};

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
