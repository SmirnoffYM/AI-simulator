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
    commodule.cpp \
    messages.cpp \
    hubmodule.cpp \
    modellingsystem.cpp \
    world.cpp \
    robot.cpp \
    envobject.cpp \
    servant.cpp \
    object.cpp \
    color.cpp \
    processcontainer.cpp \
    mapgraphicsscene.cpp \
    messagehandler.cpp

HEADERS  += mainwindow.h \
    robotwindow.h \
    commodule.h \
    messages.h \
    hubmodule.h \
    modellingsystem.h \
    world.h \
    robot.h \
    envobject.h \
    constants.h \
    servant.h \
    object.h \
    color.h \
    processcontainer.h \
    mapgraphicsscene.h \
    messagehandler.h

FORMS    += mainwindow.ui \
    robotwindow.ui

RESOURCES += \
    resource.qrc
