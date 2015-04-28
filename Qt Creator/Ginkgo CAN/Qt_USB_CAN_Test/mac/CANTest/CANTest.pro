#-------------------------------------------------
#
# Project created by QtCreator 2014-05-28T13:42:55
#
#-------------------------------------------------

QT       -= core
QT       -= gui

TARGET = CANTest
CONFIG += console
CONFIG -= app_bundle
CONFIG += static
TEMPLATE = app

DEFINES += OS_LINUX \
           USE_GCC

SOURCES += \
    ../../Source/CANTest.cpp

LIBS += -L$$PWD/lib/ -lGinkgo_Driver
LIBS += -L$$/usr/local/lib/ -lusb


HEADERS += \
    ../../Source/ControlCAN.h
