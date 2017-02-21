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

# sounds.files = sounds/shipsbell.wav
# sounds.path = $$OUT_PWD
# message("out_pwd = $$OUT_PWD")
# INSTALLS += sounds

CONFIG   += warn_on

RESOURCES += \
    resources.qrc
