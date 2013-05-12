#-------------------------------------------------
#
# Project created by QtCreator 2013-05-10T14:19:33
#
#-------------------------------------------------

QT       += core
QT       -= gui

INCLUDEPATH += "/usr/include/QtGStreamer/"

# Tell qmake to use pkg-config to find QtGStreamer.
CONFIG += link_pkgconfig

# Now tell qmake to link to QtGStreamer and also use its include path and Cflags.
PKGCONFIG += QtGStreamer-0.10 QtGStreamerUi-0.10

TARGET = kanaria
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    app.cpp

HEADERS += \
    app.h
