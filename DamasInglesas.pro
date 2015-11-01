#-------------------------------------------------
#
# Project created by QtCreator 2015-10-04T14:05:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DamasInglesas
TEMPLATE = app


SOURCES +=\
    visual/Dialogoconeccion.cpp \
    visual/mainwindow.cpp \
    visual/visualboard.cpp \
    visual/visualboardsquare.cpp \
    main.cpp \
    modelo/Tablero.cpp \
    visual/visualboardcontroler.cpp

HEADERS  += \
    visual/Dialogoconeccion.h \
    visual/mainwindow.h \
    visual/visualboard.h \
    visual/visualboardsquare.h \
    modelo/Coloreable.h \
    modelo/Ficha.h \
    modelo/Jugador.h \
    modelo/Tablero.h \
    visual/visualboardcontroler.h \
    modelo/ConstantesEstadoDelJuego.h \
    modelo/Exception.h \
    modelo/MySocket.h \
    modelo/NoSuchMessageExpectedException.h \
    modelo/socketExceptions.h \
    modelo/utilidades.h \
    modelo/utilidadesVariadas.h

FORMS    += \
    visual/Dialogoconeccion.ui \
    visual/mainwindow.ui

RESOURCES += \
    visual/res.qrc
QMAKE_CXXFLAGS += -std=c++11

DISTFILES +=
