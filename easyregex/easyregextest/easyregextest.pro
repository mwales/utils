#-------------------------------------------------
#
# Project created by QtCreator 2015-07-16T02:18:27
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_EasyregextestTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_EasyregextestTest.cpp \
    ../easyregexdemo/EasyRegex.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../easyregexdemo/EasyRegex.h

INCLUDEPATH += ../easyregexdemo
