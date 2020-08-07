QT -= gui
QT += serialport sql network dbus

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = /home/user/nmea/server

DEFINES += APP_DEBUG=1
DEFINES += TEST_VALUES

VERSION = 0.1.0    # major.minor.patch
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += VERSION_2

DBUS_ADAPTORS += niirpi.proj.nmea.xml
#DBUS_INTERFACES += ame.proj12700.ksuts.xml

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    ../global/sv_signal.cpp \
    ../../svlib/sv_pgdb.cpp \
    sv_storage.cpp \
    ../../svlib/sv_config.cpp \
    sv_storagewatcher.cpp \
    ../global/sv_dbus.cpp \
    ../../svlib/sv_abstract_logger.cpp \
    ../../svlib/sv_fnt.cpp

HEADERS += \
    ../../svlib/sv_exception.h \
    ../global/sql_defs.h \
    ../global/sv_signal.h \
    ../../svlib/sv_pgdb.h \
    sv_storage.h \
    ../global/gen_defs.h \
    ../../svlib/sv_config.h \
    ../../svlib/sv_fnt.h \
    sv_storagewatcher.h \
    ../global/sv_dbus.h \
    ../global/sv_abstract_device.h \
    ../global/device_params.h \
    ../../svlib/sv_abstract_logger.h

DISTFILES += \
    ../../../nmea/config.json \
    ../../../nmea/signals/s.json
