#-------------------------------------------------
#
# Project created by QtCreator 2012-03-01T10:54:40
#
#-------------------------------------------------

QT       += core gui network

TARGET = ai-simulator
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    robotwindow.cpp \
    message-parsers.cpp \
    commodule.cpp \
    hubmodule.cpp \
    modellingsystem.cpp \
    world.cpp \
    robot.cpp \
    envobject.cpp \
    hubthread.cpp \
    servant.cpp \
    object.cpp \
    color.cpp \
    processcontainer.cpp \
    mapgraphicsscene.cpp

HEADERS  += mainwindow.h \
    robotwindow.h \
    commodule.h \
    messages.h \
    message-parsers.h \
    hubmodule.h \
    modellingsystem.h \
    world.h \
    robot.h \
    envobject.h \
    constants.h \
    hubthread.h \
    servant.h \
    object.h \
    color.h \
    processcontainer.h \
    mapgraphicsscene.h

FORMS    += mainwindow.ui \
    robotwindow.ui

RESOURCES += \
    resource.qrc
