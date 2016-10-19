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
    BluetoothConfigWidget.cpp \
    QIOWidget.cpp \
    SerialConfigWidget.cpp \
    TcpConfigWidget.cpp \
    QIOWidgetSelector.cpp

HEADERS  += MainWindow.h \
    BluetoothConfigWidget.h \
    QIOWidget.h \
    SerialConfigWidget.h \
    TcpConfigWidget.h \
    QIOWidgetSelector.h

FORMS    += MainWindow.ui

CONFIG += mobility
MOBILITY = 

