#-------------------------------------------------
#
# Project created by QtCreator 2015-10-08T19:33:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DamasInglesas
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    visual/Dialogoconeccion.cpp \
    visual/main.cpp \
    visual/mainwindow.cpp \
    visual/visualboard.cpp \
    visual/visualboardsquare.cpp \
    Dialogoconeccion.cpp \
    visualboard.cpp \
    visualboardsquare.cpp

HEADERS  += mainwindow.h \
    visual/Dialogoconeccion.h \
    visual/mainwindow.h \
    visual/visualboard.h \
    visual/visualboardsquare.h \
    Dialogoconeccion.h \
    visualboard.h \
    visualboardsquare.h

FORMS    += mainwindow.ui
