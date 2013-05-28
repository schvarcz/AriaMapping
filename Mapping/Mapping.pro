#-------------------------------------------------
#
# Project created by QtCreator 2013-05-07T23:48:20
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mapping
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    robot.cpp \
    pidcorrection.cpp \
    wallfollowing.cpp \
    architecture.cpp \
    minimapping.cpp \
    mapping.cpp \
    cellmap.cpp \
    glwidget.cpp \
    glthread.cpp

HEADERS  += mainwindow.h \
    robot.h \
    pidcorrection.h \
    wallfollowing.h \
    architecture.h \
    minimapping.h \
    mapping.h \
    cellmap.h \
    glthread.h \
    glwidget.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/Aria/include

LIBS += -L/usr/local/Aria/lib \
-lAria -lArNetworking -lX11

RESOURCES += \
    Icons.qrc

