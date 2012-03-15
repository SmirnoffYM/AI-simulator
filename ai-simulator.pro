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
    messages-helpers.cpp

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
    messages-helpers.h

LIBS += -lqjson

FORMS    += mainwindow.ui \
    robotwindow.ui

RESOURCES += \
    resource.qrc
