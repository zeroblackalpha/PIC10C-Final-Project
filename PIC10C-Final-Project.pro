#-------------------------------------------------
#
# Project created by QtCreator 2018-05-27T22:09:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PIC10C-Final-Project
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        kmeans.cpp \
        lodepng.cpp \

HEADERS += \
        mainwindow.h \
        kmeans.h \
        lodepng.h

FORMS += \
        mainwindow.ui

RC_FILE = icon.rc

INCLUDEPATH += "C:\Users\ZeroB\Desktop\tbb-2018_U4\include"
LIBS += -L"C:\Users\ZeroB\Desktop\tbb-2018_U4\build\windows_ia32_gcc_mingw6.3.0_debug" \
        -ltbb_debug
