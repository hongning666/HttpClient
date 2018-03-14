#-------------------------------------------------
#
# Project created by QtCreator 2018-02-26T09:44:15
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HttpClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    LoginDialog.cpp

HEADERS  += mainwindow.h \
    LoginDialog.h

FORMS    += mainwindow.ui \
    LoginDialog.ui
