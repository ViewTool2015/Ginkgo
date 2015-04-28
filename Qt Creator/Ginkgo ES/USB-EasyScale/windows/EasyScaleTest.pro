#-------------------------------------------------
#
# Project created by QtCreator 2013-08-09T14:49:34
#
#-------------------------------------------------

QT       += core gui

TARGET = EasyScaleTest
TEMPLATE = app

FORMS += \
    ../source/mainwindow.ui

HEADERS += \
    ../source/EasyScale.h \
    ../source/mainwindow.h \
    ../source/ErrorType.h

SOURCES += \
    ../source/main.cpp \
    ../source/mainwindow.cpp

win32: LIBS += -L$$PWD/lib/ -lGinkgo_Driver
