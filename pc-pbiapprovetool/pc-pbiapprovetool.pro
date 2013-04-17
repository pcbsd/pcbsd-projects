QT       += core gui network

TARGET = pc-pbiapprovetool
DESTDIR = /usr/local/bin
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        pbitools.cpp

HEADERS  += mainwindow.h \
            pbitools.h

FORMS    += mainwindow.ui

RESOURCES+= pc-pbiapprovetool.qrc

LIBS     += -lQtSolutions_SingleApplication-head
