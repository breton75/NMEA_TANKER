#-------------------------------------------------
#
# Project created by QtCreator 2020-07-30T20:00:27
#
#-------------------------------------------------

QT       += network dbus

QT       -= gui

TARGET = virtual_device
TEMPLATE = lib

CONFIG += c++11 console

DEFINES += VIRTUAL_DEVICE_LIBRARY
DEFINES += VERSION_2

SOURCES += virtual_device.cpp \
    ../../svlib/sv_abstract_logger.cpp

HEADERS += virtual_device.h\
        virtual_device_global.h \
    ../global/sv_abstract_device.h \
    ../../svlib/sv_abstract_logger.h \
    my_udp_params.h \
    my_serial_params.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
