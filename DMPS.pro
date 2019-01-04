#-------------------------------------------------
#
# Project created by QtCreator 2018-11-23T14:17:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = DMPS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
LIBS +=  -lpython2.7
INCLUDEPATH +=  /usr/include/python2.7
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    dmp.cpp \
    basis.cpp \
    canonical.cpp \
    qptrajectory.cpp \
    qcustomplot.cpp

HEADERS += \
        mainwindow.h \
    dmp.h \
    basis.h \
    canonical.h \
    qptrajectory.h \
    qcustomplot.h

FORMS += \
        mainwindow.ui
