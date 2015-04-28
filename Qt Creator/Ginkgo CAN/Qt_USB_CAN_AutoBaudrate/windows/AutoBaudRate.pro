#-------------------------------------------------
#
# Project created by QtCreator 2013-08-28T17:20:52
#
#-------------------------------------------------

QT       += core gui

TARGET = AutoBaudRate
TEMPLATE = app


RESOURCES += \
    ico.qrc

OTHER_FILES += \
    ico.rc

RC_FILE = \
    ico.rc

HEADERS += \
    ../source/mainwindow.h \
    ../source/ControlCAN.h

SOURCES += \
    ../source/main.cpp \
    ../source/mainwindow.cpp

FORMS += \
    ../source/mainwindow.ui

win32: LIBS += -L$$PWD/lib/ -lGinkgo_Driver
