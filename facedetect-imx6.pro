#-------------------------------------------------
#
# Project created by QtCreator 2017-08-25T16:31:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = facedetect-imx6
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    videodevice.cpp

HEADERS  += mainwindow.h \
    videodevice.h

FORMS    += mainwindow.ui

unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv-imx6/lib/ -lopencv_core

unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv-imx6/lib/ -lopencv_highgui
unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv-imx6/lib/ -lopencv_photo
 #unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv-imx6/lib/ -lopencv_contrib
unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv-imx6/lib/ -lopencv_imgproc
#unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv-imx6/lib/ -lopencv_stitching
 unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv-imx6/lib/ -lopencv_core
#unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv-imx6/lib/ -lopencv_legacy
unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv-imx6/lib/ -lopencv_ts
 unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv-imx6/lib/ -lopencv_features2d
unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv-imx6/lib/ -lopencv_ml
unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv-imx6/lib/ -lopencv_video
 unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv-imx6/lib/ -lopencv_flann
unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv-imx6/lib/ -lopencv_nonfree
unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv-imx6/lib/ -lopencv_videostab
 unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv-imx6/lib/ -lopencv_gpu
unix:!macx: LIBS += -L$$PWD/../../../../usr/local/opencv-imx6/lib/ -lopencv_objdetect

INCLUDEPATH += $$PWD/../../../../usr/local/opencv-imx6/include
DEPENDPATH += $$PWD/../../../../usr/local/opencv-imx6/include
