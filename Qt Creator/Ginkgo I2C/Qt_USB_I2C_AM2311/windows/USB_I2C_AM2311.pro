#-------------------------------------------------
#
# Project created by QtCreator 2014-02-10T11:09:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ginkgo_USB-I2C_AM2311
TEMPLATE = app

INCLUDEPATH += ../source/AnalogWidgets/analogwidgets/analogwidgets
INCLUDEPATH += C:/Qt/qwt-6.1.0/include
win32:LIBS += -L$$PWD/lib/ -lanalogwidgets
win32:LIBS += -L$$PWD/lib/ -lGinkgo_Driver
win32:LIBS += -L$$PWD/lib/ -lqwt
FORMS += \
    ../source/mainwindow.ui

HEADERS += \
    ../source/mainwindow.h \
    ../source/ControlI2C.h \
    ../source/ErrorType.h

SOURCES += \
    ../source/main.cpp \
    ../source/mainwindow.cpp

OTHER_FILES += \
    ../source/ico.rc
RC_FILE = \
    ../source/ico.rc
