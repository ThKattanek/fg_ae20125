#-------------------------------------------------
#
# Project created by QtCreator 2014-08-31T10:27:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fg_ae20125
TEMPLATE = app

linux-g++-32{
DESTDIR = "bin/linux_32bit"
}

linux-g++-64{
DESTDIR = "bin/linux_64bit"
}

win32-g++{
DESTDIR = "bin/win_x32"
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h
FORMS    += mainwindow.ui

