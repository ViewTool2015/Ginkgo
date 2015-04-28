#-------------------------------------------------
#
# Project created by QtCreator 2013-05-14T11:49:55
#
#-------------------------------------------------

QT       += core gui

TARGET = MPU6050
TEMPLATE = app

FORMS += \
    ../source/mainwindow.ui

HEADERS += \
    ../source/ControlI2C.h \
    ../source/ErrorType.h \
    ../source/mainwindow.h

SOURCES += \
    ../source/main.cpp \
    ../source/mainwindow.cpp


INCLUDEPATH += C:/Qt/qwt-6.1.0/src

LIBS += -L"C:/Qt/qwt-6.1.0/lib" -lqwt
win32: LIBS += -L$$PWD/lib/ -lGinkgo_Driver

RC_FILE = \
    ../source/ico.rc

OTHER_FILES += \
    ../source/ico.rc



