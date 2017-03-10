#-------------------------------------------------
#
# Project created by QtCreator 2017-02-21T17:51:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    image.cpp \
    coloranalyzer.cpp \
    kinematicsprovider.cpp \
    vector2D.cpp \
    EdgeImage.cpp \
    FRHT.cpp

HEADERS  += mainwindow.h \
    image.h \
    coloranalyzer.h \
    kinematicsprovider.h \
    vector2D.h \
    EdgeImage.h \
    FRHT.h

FORMS    += mainwindow.ui
