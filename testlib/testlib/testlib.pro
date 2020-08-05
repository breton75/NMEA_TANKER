#-------------------------------------------------
#
# Project created by QtCreator 2020-08-04T20:47:46
#
#-------------------------------------------------

QT       -= gui

TARGET = /home/user/nmea/lib/testlib
TEMPLATE = lib

CONFIG += c++11

DEFINES += TESTLIB_LIBRARY

SOURCES += testlib.cpp

HEADERS += testlib.h\
        testlib_global.h \
    ../testapp/abstract_test.h \
    ../../global/sv_abstract_device.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
