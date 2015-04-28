#-------------------------------------------------
#
# Project created by QtCreator 2014-01-16T16:08:22
#
#-------------------------------------------------

QT       += core gui

TARGET = CAN_Bootloader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    ErrorType.h \
    ControlCAN.h

FORMS    += mainwindow.ui

LIBS += -L./lib/ -lGinkgo_Driver
LIBS += -L./lib/ -llibusb

OTHER_FILES += \
    ico.rc
RC_FILE = \
    ico.rc
