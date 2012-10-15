#include "messages.h"

/**** MessageMove
 ******************************************/
MessageMove::MessageMove() { type = MsgMove; }

MessageMove::MessageMove(QDataStream &stream) {
    // I would have wrote that in C++11 from the beginning if I knew there was delegating
    // constructors in the upcoming standard
    type = MsgMove;

    // it's safe to read directly into unsigned int without casting to quint32 - the latter is
    // just a typedef aliasing to the former
    stream >> coordX >> coordY;
}

/**** MessageTurn
 ******************************************/
MessageTurn::MessageTurn() { type = MsgTurn; }

MessageTurn::MessageTurn(QDataStream &stream) {
    type = MsgTurn;

    qint32 seconds;
    stream >> seconds;

    // 60 seconds is a minute, and 60 minutes is a degree
    degrees = static_cast<double>(seconds) / 3600;
}

/**** MessageChangeSize
 ******************************************/
MessageChangeSize::MessageChangeSize() { type = MsgChangeSize; }

MessageChangeSize::MessageChangeSize(QDataStream &stream) {
    type = MsgChangeSize;

    stream >> diameter;
}

/**** MessageChangeColor
 ******************************************/
MessageChangeColor::MessageChangeColor() { type = MsgChangeColor; }

MessageChangeColor::MessageChangeColor(QDataStream &stream) {
    type = MsgChangeColor;

    // have to use intermediate variables as there's no operator>>(char) defined for QDataStream
    quint8 r, g, b;
    stream >> r >> g >> b;

    red = r;
    green = g;
    blue = b;
}

/**** MessageWhoIsThere
 ******************************************/
MessageWhoIsThere::MessageWhoIsThere() { type = MsgWhoIsThere; }

MessageWhoIsThere::MessageWhoIsThere(QDataStream &stream) {
    type = MsgWhoIsThere;

    stream >> coordX >> coordY >> radius;
}

/**** MessageBump
 ******************************************/
MessageBump::MessageBump() { type = MsgBump; }

/**** MessageHit
 ******************************************/
MessageHit::MessageHit() { type = MsgHit; }

/**** MessageMovedSuccessfully
 ******************************************/
MessageMovedSuccessfully::MessageMovedSuccessfully() { type = MsgMovedSuccessfully; }

/**** MessageNavigationChart
 ******************************************/
MessageNavigationChart::MessageNavigationChart() {
    type = MsgNavigationChart;
}

/**** MessageParameterReport
 ******************************************/
MessageParameterReport::MessageParameterReport() { type = MsgParameterReport; }

MessageParameterReport::MessageParameterReport(QDataStream &stream) {
    type = MsgParameterReport;

    quint8 i; // id
    stream >> i
           >> integral >> real;

    id = i;
}

/**** MessageThereYouSee
 ******************************************/
MessageThereYouSee::MessageThereYouSee() { type = MsgThereYouSee; }

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
