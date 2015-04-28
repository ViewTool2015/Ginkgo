#-------------------------------------------------
#
# Project created by QtCreator 2013-12-24T14:38:57
#
#-------------------------------------------------

QT       += core gui

TARGET = ADS1232
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    ControlSPI.h

FORMS    += mainwindow.ui

LIBS += -L./lib/ -lGinkgo_Driver
LIBS += -L./lib/ -llibusb
