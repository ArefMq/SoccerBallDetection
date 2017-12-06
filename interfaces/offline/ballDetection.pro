#-------------------------------------------------
#
# Project created by QtCreator 2017-03-21T09:52:58
#
#-------------------------------------------------

QT       += core gui
DEFINES += DEBUG

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ballDetection
TEMPLATE = app

LIBS += -lm \
        -lopencv_core \
        -lopencv_imgproc \
        -lopencv_highgui \
        -lopencv_ml \
        -lopencv_video \
        -lopencv_features2d \
        -lopencv_calib3d \
        -lopencv_objdetect \
        -lopencv_contrib \
        -lopencv_legacy \
        -lopencv_stitching \
        -lpython2.7 \
        -L/usr/lib/x86_64-linux-gnu/

INCLUDEPATH += ../../src/ ../../src/tools/ /usr/include/python2.7/

SOURCES += main.cpp\
        mainwindow.cpp \
    ../../src/tools/image.cpp \
    ../../src/balldetector.cpp \
    ../../src/tools/circle.cpp \
    ../../src/tools/ball.cpp \
    ../../src/tools/vector2D.cpp \
    ../../src/modules/coloranalyzer.cpp \
    ../../src/modules/edgeimage.cpp \
    ../../src/modules/FRHT.cpp \
    ../../src/modules/kinematicsprovider.cpp \
    ../../src/modules/patternrecognizer.cpp \
    ../../src/tools/vector3D.cpp \
    ../../src/modules/ml/mmlwrapper.cpp

HEADERS  += mainwindow.h \
    ../../src/tools/image.h \
    ../../src/balldetector.h \
    ../../src/tools/circle.h \
    ../../src/tools/ball.h \
    ../../src/tools/vector2D.h \
    ../../src/modules/coloranalyzer.h \
    ../../src/modules/edgeimage.h \
    ../../src/modules/FRHT.h \
    ../../src/modules/kinematicsprovider.h \
    ../../src/modules/patternrecognizer.h \
    ../../src/tools/vector3D.h \
    ../../src/modules/ml/mmlwrapper.h \
    util.h

FORMS    += mainwindow.ui

