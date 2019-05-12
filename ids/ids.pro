#-------------------------------------------------
#
# Project created by QtCreator 2019-05-01T18:56:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ids
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

CONFIG += c++11

SOURCES += \
        idscam.cpp \
        life.cpp \
        main.cpp \
        qcustomplot.cpp \
        testcam.cpp \
        widget.cpp

HEADERS += \
    cam.h \
    idscam.h \
    life.h \
    qcustomplot.h \
    testcam.h \
        widget.h \
    C:/Program Files/IDS/uEye/Develop/include/uEye.h \
    C:/Program Files/IDS/uEye/Develop/include/uEye.h

FORMS += \
        widget.ui


unix|win32: LIBS += -L'C:/Program Files/IDS/uEye/Develop/Lib/' -luEye_api
unix|win32: LIBS += -L'C:/Program Files/IDS/uEye/Develop/Lib/' -luEye_api_64
INCLUDEPATH += 'C:/Program Files/IDS/uEye/Develop/include'
DEPENDPATH += 'C:/Program Files/IDS/uEye/Develop/include'

