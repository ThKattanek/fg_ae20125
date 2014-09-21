#-------------------------------------------------
#
# Project created by QtCreator 2014-08-31T10:27:48
#
#-------------------------------------------------

#-------------------------------------------------
# PFADE hier setzen
#-------------------------------------------------

#-- Windows Installpfad
WIN_INSTALL_DIR = install

QT       += core gui

TRANSLATIONS = translations/lang_de.ts
TRANSLATIONS = translations/lang_en.ts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fg_ae20125
TEMPLATE = app

DESTDIR = "bin"

win32-g++{
DESTDIR = "bin/win_x32"
}

SOURCES += main.cpp\
        mainwindow.cpp \
    setup_dialog.cpp \
    ascel_protokoll.cpp \
    license_dialog.cpp \
    info_dialog.cpp

HEADERS  += \
    setup_dialog.h \
    mainwindow.h \
    ascel_protokoll.h \
    version.h \
    license_dialog.h \
    info_dialog.h
FORMS    += mainwindow.ui \
    setup_dialog.ui \
    license_dialog.ui \
    info_dialog.ui

LIBS += -lQtSerialPort

OTHER_FILES += \
    README.md \
    translations/lang_de.ts \
    translations/lang_en.ts \
    translations/lang_de.png \
    translations/lang_en.png \
    docs/daten_protokoll_ascel.pdf \
    license.txt \
    translations/lang_en.qm \
    translations/lang_de.qm

## Translation Release ##

QMAKE_PRE_LINK += lrelease translations/lang_de.ts \
			& lrelease translations/lang_en.ts

## INSTALL LINUX (make install) ##

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

## INSTALL WINDOWS (mingw32-make install) ##

# QMAKE_LIBDIR_QT = C:\Entwicklertools\Qt\Qt5.2.1\5.2.1\mingw48_32\bin

win32-g++{
QMAKE_LFLAGS_WINDOWS = -static-libgcc

target.path =  $$WIN_INSTALL_DIR
helpfiles.path = $$WIN_INSTALL_DIR
#helpfiles.files = bin/help/*
#helpfiles.files += bin/help/html/*.html
#desktop.path =  /usr/share/applications
#desktop.files = fg_ae20125.desktop
#icon.path =  /usr/share/pixmaps
#icon.files = fg_ae20125.svg
#languages.path = /usr/share/fg_ae20125
languages.path = $$WIN_INSTALL_DIR/lang
languages.files = translations/*.qm
languages.files += translations/*.png

mylibs.path = $$WIN_INSTALL_DIR
mylibs.files = $${QMAKE_LIBDIR_QT}/Qt*.dll
mylibs.CONFIG = no_check_exist

INSTALLS = target
#INSTALLS += helpfiles
#INSTALLS += desktop
#INSTALLS += icon
INSTALLS += languages
INSTALLS += mylibs
}

RESOURCES += \
    resources.qrc
