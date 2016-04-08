#-------------------------------------------------
#
# Project created by QtCreator 2015-07-04T01:47:13
#
#-------------------------------------------------

QT       += core gui network multimedia multimediawidgets xml widgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xaman
TEMPLATE = app


SOURCES += main.cpp\
        xaman.cpp \
    player.cpp \
    playercontrols.cpp \
    playlistmodel.cpp \
    videowidget.cpp \
    logindialog.cpp \
    agregardialog.cpp \
    comunicacion.cpp \
    dialogaddlista.cpp \
    sender.cpp \
    load.cpp \
    dialogadmin.cpp \
    toolbutton.cpp \
    tcpsocket.cpp

HEADERS  += xaman.h \
    player.h \
    playercontrols.h \
    playlistmodel.h \
    videowidget.h \
    logindialog.h \
    agregardialog.h \
    comunicacion.h \
    dialogaddlista.h \
    sender.h \
    load.h \
    dialogadmin.h \
    toolbutton.h \
    tcpsocket.h

FORMS    += xaman.ui \
    logindialog.ui \
    agregardialog.ui \
    dialogaddlista.ui \
    load.ui \
    dialogadmin.ui

RESOURCES += \
    resource.qrc
