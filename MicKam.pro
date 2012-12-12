#-------------------------------------------------
#
# Project created by QtCreator 2012-05-18T12:34:13
#
#-------------------------------------------------

QT       += core gui
QT -= console

TARGET = mikcam
TEMPLATE = app
MOC_DIR = moc
OBJECTS_DIR = obj
DESTDIR += bin
UI_DIR = ui
UI_SOURCES_DIR = ui
UI_HEADERS_DIR = ui
VERSION = 0.0.0.0
CONFIG += debug_and_release
win32 {
}
unix{
LIBS += -L/usr/lib/x86_64-linux-gnu/
LIBS += -lgphoto2
LIBS += -L./SO/ -lqdsandfc# -ldetectorscan
#LIBS += -L/usr/lib
#LIBS += -ldetectorscan
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann
}

SOURCES += main.cpp\
        mickammainwindow.cpp

HEADERS  += mickammainwindow.h

FORMS    += ./ui/mickammainwindow.ui
