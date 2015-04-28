#-------------------------------------------------
#
# Project created by QtCreator 2014-02-10T11:09:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ginkgo_USB-I2C_BMP180
TEMPLATE = app

INCLUDEPATH += ../source/AnalogWidgets/analogwidgets/analogwidgets

win32:LIBS += -L$$PWD/lib/ -lanalogwidgets
win32:LIBS += -L$$PWD/lib/ -lGinkgo_Driver
FORMS += \
    ../source/mainwindow.ui

HEADERS += \
    ../source/mainwindow.h \
    ../source/ControlI2C.h \
    ../source/ErrorType.h \
    ../source/bmp180.h

SOURCES += \
    ../source/main.cpp \
    ../source/mainwindow.cpp \
    ../source/bmp180.cpp

OTHER_FILES += \
    ../source/ico.rc
RC_FILE = \
    ../source/ico.rc
