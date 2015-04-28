################################################################
# Qwt Widget Library
# Copyright (C) 1997   Josef Wilgen
# Copyright (C) 2002   Uwe Rathmann
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Qwt License, Version 1.0
################################################################

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = oscilloscope
TEMPLATE = app

HEADERS = \
    ../../source/curvedata.h \
    ../../source/knob.h \
    ../../source/mainwindow.h \
    ../../source/plot.h \
    ../../source/samplingthread.h \
    ../../source/signaldata.h \
    ../../source/wheelbox.h

SOURCES = \
    ../../source/curvedata.cpp \
    ../../source/knob.cpp \
    ../../source/main.cpp \
    ../../source/mainwindow.cpp \
    ../../source/plot.cpp \
    ../../source/samplingthread.cpp \
    ../../source/signaldata.cpp \
    ../../source/wheelbox.cpp

INCLUDEPATH += C:/Qt/qwt-6.1.0/src

LIBS += -L$$PWD/lib/ -lqwt
LIBS += -L$$PWD/lib/ -lGinkgo_Driver
