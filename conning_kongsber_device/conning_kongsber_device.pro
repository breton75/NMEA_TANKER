#-------------------------------------------------
#
# Project created by QtCreator 2020-07-30T20:00:27
#
#-------------------------------------------------

QT       += network dbus serialport

QT       -= gui

TARGET = /home/user/nmea/lib/conning_kongsber
TEMPLATE = lib

CONFIG += c++11

#чтобы не цеплять версию к имени файла
#CONFIG += plugin

DEFINES += CONNING_KONGSBER_LIBRARY
#DEFINES += VERSION_2

SOURCES += conning_kongsber_device.cpp \
    ../../svlib/sv_abstract_logger.cpp \
    ../global/sv_signal.cpp

HEADERS += conning_kongsber_device.h\
        conning_kongsber_device_global.h \
    ../global/sv_abstract_device.h \
    ../../svlib/sv_abstract_logger.h \
    device_params.h \
    ifc_udp_params.h \
    ifc_serial_params.h \
    ifc_test_params.h \
    signal_params.h \
    ../global/sv_signal.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
