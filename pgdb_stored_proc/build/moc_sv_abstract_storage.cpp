/****************************************************************************
** Meta object code from reading C++ file 'sv_abstract_storage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../global/sv_abstract_storage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sv_abstract_storage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_as__SvAbstractStorage_t {
    QByteArrayData data[1];
    char stringdata0[22];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_as__SvAbstractStorage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_as__SvAbstractStorage_t qt_meta_stringdata_as__SvAbstractStorage = {
    {
QT_MOC_LITERAL(0, 0, 21) // "as::SvAbstractStorage"

    },
    "as::SvAbstractStorage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_as__SvAbstractStorage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void as::SvAbstractStorage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject as::SvAbstractStorage::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_as__SvAbstractStorage.data,
      qt_meta_data_as__SvAbstractStorage,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *as::SvAbstractStorage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *as::SvAbstractStorage::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_as__SvAbstractStorage.stringdata0))
        return static_cast<void*>(const_cast< SvAbstractStorage*>(this));
    return QObject::qt_metacast(_clname);
}

int as::SvAbstractStorage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_as__SvAbstractStorageThread_t {
    QByteArrayData data[1];
    char stringdata0[28];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_as__SvAbstractStorageThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_as__SvAbstractStorageThread_t qt_meta_stringdata_as__SvAbstractStorageThread = {
    {
QT_MOC_LITERAL(0, 0, 27) // "as::SvAbstractStorageThread"

    },
    "as::SvAbstractStorageThread"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_as__SvAbstractStorageThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void as::SvAbstractStorageThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject as::SvAbstractStorageThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_as__SvAbstractStorageThread.data,
      qt_meta_data_as__SvAbstractStorageThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *as::SvAbstractStorageThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *as::SvAbstractStorageThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_as__SvAbstractStorageThread.stringdata0))
        return static_cast<void*>(const_cast< SvAbstractStorageThread*>(this));
    return QThread::qt_metacast(_clname);
}

int as::SvAbstractStorageThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
