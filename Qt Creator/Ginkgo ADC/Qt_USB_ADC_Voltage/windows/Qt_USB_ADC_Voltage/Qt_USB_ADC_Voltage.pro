#-------------------------------------------------
#
# Project created by QtCreator 2013-04-06T11:00:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = USB-ADC_Voltage
TEMPLATE = app


SOURCES += \
    ../../source/main.cpp \
    ../../source/mainwindow.cpp

HEADERS  += \
    ../../source/ControlADC.h \
    ../../source/ErrorType.h \
    ../../source/mainwindow.h

FORMS    += \
    ../../source/mainwindow.ui

OTHER_FILES += \
    ../../source/ico.rc
RC_FILE = \
    ../../source/ico.rc

LIBS += -L$$PWD/lib/ -lGinkgo_Driver

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/


