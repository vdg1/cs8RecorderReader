#-------------------------------------------------
#
# Project created by QtCreator 2014-12-20T22:00:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cs8RecorderReader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cs8recordermodel.cpp \
    formprojectdata.cpp \
cs8polygonlimit.cpp \
    formpolygonlimiteditor.cpp

HEADERS  += mainwindow.h \
    cs8recordermodel.h \
    formprojectdata.h \
    cs8polygonlimit.h \
    formpolygonlimiteditor.h \
    XMLSettings.h

FORMS    += mainwindow.ui \
    formpolygonlimiteditor.ui \
    formprojectdata.ui

