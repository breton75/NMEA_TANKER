#-------------------------------------------------
#
# Project created by QtCreator 2020-08-06T16:47:06
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = pgdb_stored_proc
TEMPLATE = lib

DEFINES += PGDB_STORED_PROC_LIBRARY

SOURCES += pgdb_stored_proc.cpp

HEADERS += pgdb_stored_proc.h\
        pgdb_stored_proc_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
