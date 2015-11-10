#-------------------------------------------------
#
# Project created by QtCreator 2015-10-20T18:02:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cvip
TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp\
        qt/mainwindow.cpp

HEADERS  += qt/mainwindow.h \
    src/cvip.h

FORMS    += qt/mainwindow.ui

RESOURCES +=

DISTFILES += \
    img/tux.png \
    README.md
