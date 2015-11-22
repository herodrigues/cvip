#-------------------------------------------------
#
# Project created by QtCreator 2015-10-20T18:02:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cvip
TEMPLATE = app

win32 {
    TARGET = cvip.exe
    INCLUDEPATH += "C:\Qt\Qt5.5.1\5.5\mingw492_32\include\opencv"
    LIBS += -L"C:\MinGW\lib" -lopencv_core -lopencv_imgcodecs -lopencv_highgui
}

unix {
    TARGET = cvip
    INCLUDEPATH += /usr/local/include/opencv
    LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui
}

QMAKE_CXXFLAGS += -std=c++0x -Wswitch-enum

SOURCES += main.cpp\
        qt/mainwindow.cpp

HEADERS  += qt/mainwindow.h \
    src/cvip.h

FORMS    += qt/mainwindow.ui

RESOURCES +=

DISTFILES += \
    img/tux.png \
    README.md \
    .gitignore \
    doc/*
