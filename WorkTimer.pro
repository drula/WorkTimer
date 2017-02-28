#-------------------------------------------------
#
# Project created by QtCreator 2016-12-17T20:37:12
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WorkTimer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

CONFIG   += warn_on

RESOURCES += \
    resources.qrc

TRANSLATIONS += translations/mainwindow_by.ts
