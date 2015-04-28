#-------------------------------------------------
#
# Project created by QtCreator 2014-02-07T17:17:30
#
#-------------------------------------------------

QT       += core gui

TARGET = USB_PWM_Test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    ErrorType.h \
    ControlPWM.h

FORMS    += mainwindow.ui

win32:LIBS += -L$$PWD/lib/ -lGinkgo_Driver

RESOURCES +=

OTHER_FILES += \
    source/ico.rc
RC_FILE = \
    source/ico.rc
