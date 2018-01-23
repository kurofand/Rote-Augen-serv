#-------------------------------------------------
#
# Project created by QtCreator 2017-09-03T10:05:17
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = screenServer
TEMPLATE = app


SOURCES += main.cpp\
        screenservermain.cpp \
    server.cpp \
    serversclient.cpp

HEADERS  += screenservermain.h \
    server.h \
    serversclient.h

FORMS    += screenservermain.ui
