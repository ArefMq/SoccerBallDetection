#-------------------------------------------------
#
# Project created by QtCreator 2017-03-21T09:52:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ballDetection
TEMPLATE = app

INCLUDEPATH += ../../src/ ../../src/tools/

SOURCES += main.cpp\
        mainwindow.cpp \
    ../../src/tools/image.cpp \
    ../../src/balldetector.cpp \
    ../../src/tools/circle.cpp \
    ../../src/tools/ball.cpp \
    ../../src/tools/vector2D.cpp \
    ../../src/modules/coloranalyzer.cpp \
    ../../src/modules/EdgeImage.cpp \
    ../../src/modules/FRHT.cpp \
    ../../src/modules/kinematicsprovider.cpp \
    ../../src/tools/vector3D.cpp

HEADERS  += mainwindow.h \
    ../../src/tools/image.h \
    ../../src/balldetector.h \
    ../../src/tools/circle.h \
    ../../src/tools/ball.h \
    ../../src/tools/vector2D.h \
    ../../src/modules/coloranalyzer.h \
    ../../src/modules/EdgeImage.h \
    ../../src/modules/FRHT.h \
    ../../src/modules/kinematicsprovider.h \
    ../../src/tools/vector3D.h

FORMS    += mainwindow.ui
