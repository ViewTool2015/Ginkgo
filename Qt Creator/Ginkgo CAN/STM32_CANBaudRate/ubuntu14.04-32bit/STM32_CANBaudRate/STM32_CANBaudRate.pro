#-------------------------------------------------
#
# Project created by QtCreator 2014-06-05T09:31:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = STM32_CANBaudRate
TEMPLATE = app


SOURCES += \
    ../../Source/mainwindow.cpp \
    ../../Source/main.cpp

HEADERS  += \
    ../../Source/mainwindow.h

FORMS    += \
    ../../Source/mainwindow.ui

OTHER_FILES += \
    ico.rc

RC_FILE = \
    ico.rc

RESOURCES += \
    ico.qrc
