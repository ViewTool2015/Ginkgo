#-------------------------------------------------
#
# Project created by QtCreator 2015-03-05T13:28:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WaveformGenerator
TEMPLATE = app

FORMS += \
    ../../source/mainwindow.ui

HEADERS += \
    ../../source/mainwindow.h \
    ../../source/ControlDAC.h \
    ../../source/ErrorType.h

SOURCES += \
    ../../source/main.cpp \
    ../../source/mainwindow.cpp

INCLUDEPATH += C:/Qt/qwt-6.1.0/src
LIBS += -L$$PWD/lib/ -lqwt
LIBS += -L$$PWD/lib/ -lGinkgo_Driver

RESOURCES += \
    ../../source/icon.qrc

OTHER_FILES += \
    ../../source/ico.rc

RC_FILE = ../../source/ico.rc
