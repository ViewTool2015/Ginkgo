#-------------------------------------------------
#
# Project created by QtCreator 2014-07-04T16:46:32
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = CANTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    ../../../Source/CANTest.cpp

HEADERS += \
    ../../../Source/ControlCAN.h

win32: LIBS += -L$$PWD/lib/ -lGinkgo_Driver
