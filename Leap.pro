#-------------------------------------------------
#
# Project created by QtCreator 2016-07-18T09:11:32
#
#-------------------------------------------------

macx: {
    QMAKE_MAC_SDK = macosx10.12
}

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += ./

unix:!macx {
    LIBS += ../LeapPaint/lib/libLeap.so
}
macx: {
    LIBS += ../LeapPaint/lib/libLeap.dylib
}

TARGET = LeapPaint
TEMPLATE = app

SOURCES += main.cpp \
    controllerlistener.cpp \
    canvas.cpp \
    mywindow.cpp

HEADERS  += mywindow.h \
    controllerlistener.h \
    canvas.h \
    fingervector.h

FORMS    += mywindow.ui \
    mywindow.ui
