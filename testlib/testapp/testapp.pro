QT += core
QT -= gui

TARGET = testapp
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    abstract_test.h \
    ../../global/sv_abstract_device.h

