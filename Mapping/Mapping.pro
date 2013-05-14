#-------------------------------------------------
#
# Project created by QtCreator 2013-05-07T23:48:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mapping
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    robot.cpp \
    pidcorrection.cpp \
    wallfollowing.cpp \
    architecture.cpp \
    mapping.cpp

HEADERS  += mainwindow.h \
    robot.h \
    pidcorrection.h \
    wallfollowing.h \
    architecture.h \
    mapping.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/Aria/include

LIBS += -L/usr/local/Aria/lib \
-lAria -lArNetworking
