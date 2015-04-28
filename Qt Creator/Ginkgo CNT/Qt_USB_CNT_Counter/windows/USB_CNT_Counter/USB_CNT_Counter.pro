#-------------------------------------------------
#
# Project created by QtCreator 2014-07-08T12:29:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = USB_CNT_Counter
TEMPLATE = app


SOURCES += \
    ../../source/main.cpp \
    ../../source/mainwindow.cpp

HEADERS  += \
    ../../source/mainwindow.h \
    ../../source/ControlCNT.h \
    ../../source/ErrorType.h

FORMS    += \
    ../../source/mainwindow.ui

win32:LIBS += -L$$PWD/lib/ -lGinkgo_Driver

OTHER_FILES += \
    ../../source/ico.rc
RC_FILE = \
    ../../source/ico.rc
