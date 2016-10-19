#-------------------------------------------------
#
# Project created by QtCreator 2016-10-19T01:31:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DeviceTerminal
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    BluetoothServiceWidget.cpp \
    QIOWidget.cpp \
    SerialConfigWidget.cpp \
    TcpDestinationWidget.cpp \
    QIOWidgetSelector.cpp

HEADERS  += MainWindow.h \
    BluetoothServiceWidget.h \
    QIOWidget.h \
    SerialConfigWidget.h \
    TcpDestinationWidget.h \
    QIOWidgetSelector.h

FORMS    += MainWindow.ui

CONFIG += mobility
MOBILITY = 

