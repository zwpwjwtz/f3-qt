#-------------------------------------------------
#
# Project created by QtCreator 2016-03-23T17:54:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = f3-qt
TEMPLATE = app

DEFINES += QT_DISABLE_DEPRECATED_UP_TO=0x050F00

VER_MAJ = 2
VER_MIN = 2
VER_PAT = 1
VERSION = 2.2.1
VERSION_PE_HEADER = 2.2.1

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

SOURCES += main.cpp\
        mainwindow.cpp \
    f3_launcher.cpp \
    helpwindow.cpp

HEADERS  += mainwindow.h \
    f3_launcher.h \
    helpwindow.h

FORMS    += mainwindow.ui \
    helpwindow.ui

DISTFILES +=

RESOURCES += \
    icon.qrc \
    translation.qrc

target.path = $${PREFIX}/bin/

TRANSLATIONS += \
    translations/f3-qt_en_US.ts
    translations/f3-qt_zh_CN.ts

INSTALLS += target
