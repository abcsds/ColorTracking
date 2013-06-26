#-------------------------------------------------
#
# Project created by QtCreator 2013-04-12T01:11:04
#
#-------------------------------------------------

QT       += core gui

TARGET = VR01
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cv2qt.cpp

HEADERS  += mainwindow.h \
    cv2qt.h

FORMS    += mainwindow.ui


INCLUDEPATH += /opt/local/include \

LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann

