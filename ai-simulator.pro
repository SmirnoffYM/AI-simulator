#-------------------------------------------------
#
# Project created by QtCreator 2012-03-01T10:54:40
#
#-------------------------------------------------

QT       += core gui

TARGET = ai-simulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    robotwindow.cpp \
    commodule.cpp \
    hubmodule.cpp \
    modellingsystem.cpp \
    world.cpp \
    robot.cpp \
    envobject.cpp \
    qhubthread.cpp \
    qcomthread.cpp

HEADERS  += mainwindow.h \
    robotwindow.h \
    commodule.h \
    hubmodule.h \
    modellingsystem.h \
    world.h \
    robot.h \
    envobject.h \
    constants.h \
    qhubthread.h \
    qcomthread.h

FORMS    += mainwindow.ui \
    robotwindow.ui
