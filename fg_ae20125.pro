#-------------------------------------------------
#
# Project created by QtCreator 2014-08-31T10:27:48
#
#-------------------------------------------------

QT       += core gui

TRANSLATIONS = translations/lang_de.ts
TRANSLATIONS = translations/lang_en.ts

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

OTHER_FILES += \
    README.md \
    translations/lang_de.ts \
    translations/lang_en.ts \
    translations/lang_de.png \
    translations/lang_en.png \
    docs/daten_protokoll_ascel.pdf

linux-g++-64{    
target.path +=  /usr/bin/
data.path += /usr/share/fg_ae20125

helpfiles.path += /usr/share/fg_ae20125
#helpfiles.files += bin/help/*
#helpfiles.files += bin/help/html/*.html
desktop.path +=  /usr/share/applications
desktop.files += fg_ae20125.desktop
icon.path +=  /usr/share/pixmaps
icon.files += fg_ae20125.svg
languages.path += /usr/share/fg_ae20125/lang
languages.files += translations/*.qm
languages.files += translations/*.png
INSTALLS += target
INSTALLS += data
INSTALLS += helpfiles
INSTALLS += desktop
INSTALLS += icon    
INSTALLS += languages
}
