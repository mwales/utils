#-------------------------------------------------
#
# Project created by QtCreator 2017-07-29T18:38:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QMapWidget
TEMPLATE = app


SOURCES += main.cpp\
        MapTestApp.cpp \
    MapMgr.cpp

HEADERS  += MapTestApp.h \
    MapMgr.h

FORMS    += MapTestApp.ui

CONFIG += c++11

CartotypeSdkLocation = /home/mwales/Downloads/cartotype-ubuntu-sdk

HEADERS += $$CartotypeSdkLocation/header

INCLUDEPATH += $$CartotypeSdkLocation/header

LIBS += $$CartotypeSdkLocation/bin/libcartotype.a


