#-------------------------------------------------
#
# Project created by QtCreator 2014-01-06T11:13:08
#
#-------------------------------------------------

QT       += core gui

TARGET = USB_I2C_EEPROM_ID
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    ErrorType.h \
    ControlI2C.h

FORMS    += mainwindow.ui

win32:LIBS += -L$$PWD/lib/ -lGinkgo_Driver
