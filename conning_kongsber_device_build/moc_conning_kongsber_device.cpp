/****************************************************************************
** Meta object code from reading C++ file 'conning_kongsber_device.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../conning_kongsber_device/conning_kongsber_device.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'conning_kongsber_device.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ConningKongsberDevice_t {
    QByteArrayData data[4];
    char stringdata0[47];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConningKongsberDevice_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConningKongsberDevice_t qt_meta_stringdata_ConningKongsberDevice = {
    {
QT_MOC_LITERAL(0, 0, 21), // "ConningKongsberDevice"
QT_MOC_LITERAL(1, 22, 10), // "stopThread"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 12) // "deleteThread"

    },
    "ConningKongsberDevice\0stopThread\0\0"
    "deleteThread"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConningKongsberDevice[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   25,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void ConningKongsberDevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConningKongsberDevice *_t = static_cast<ConningKongsberDevice *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stopThread(); break;
        case 1: _t->deleteThread(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ConningKongsberDevice::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConningKongsberDevice::stopThread)) {
                *result = 0;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ConningKongsberDevice::staticMetaObject = {
    { &ad::SvAbstractDevice::staticMetaObject, qt_meta_stringdata_ConningKongsberDevice.data,
      qt_meta_data_ConningKongsberDevice,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ConningKongsberDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConningKongsberDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ConningKongsberDevice.stringdata0))
        return static_cast<void*>(const_cast< ConningKongsberDevice*>(this));
    return ad::SvAbstractDevice::qt_metacast(_clname);
}

int ConningKongsberDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ad::SvAbstractDevice::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ConningKongsberDevice::stopThread()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_ConningKongsberGenericThread_t {
    QByteArrayData data[1];
    char stringdata0[29];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConningKongsberGenericThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConningKongsberGenericThread_t qt_meta_stringdata_ConningKongsberGenericThread = {
    {
QT_MOC_LITERAL(0, 0, 28) // "ConningKongsberGenericThread"

    },
    "ConningKongsberGenericThread"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConningKongsberGenericThread[] = {

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

void ConningKongsberGenericThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject ConningKongsberGenericThread::staticMetaObject = {
    { &ad::SvAbstractDeviceThread::staticMetaObject, qt_meta_stringdata_ConningKongsberGenericThread.data,
      qt_meta_data_ConningKongsberGenericThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ConningKongsberGenericThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConningKongsberGenericThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ConningKongsberGenericThread.stringdata0))
        return static_cast<void*>(const_cast< ConningKongsberGenericThread*>(this));
    return ad::SvAbstractDeviceThread::qt_metacast(_clname);
}

int ConningKongsberGenericThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ad::SvAbstractDeviceThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_ConningKongsberUDPThread_t {
    QByteArrayData data[3];
    char stringdata0[31];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConningKongsberUDPThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConningKongsberUDPThread_t qt_meta_stringdata_ConningKongsberUDPThread = {
    {
QT_MOC_LITERAL(0, 0, 24), // "ConningKongsberUDPThread"
QT_MOC_LITERAL(1, 25, 4), // "stop"
QT_MOC_LITERAL(2, 30, 0) // ""

    },
    "ConningKongsberUDPThread\0stop\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConningKongsberUDPThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void ConningKongsberUDPThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConningKongsberUDPThread *_t = static_cast<ConningKongsberUDPThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stop(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ConningKongsberUDPThread::staticMetaObject = {
    { &ConningKongsberGenericThread::staticMetaObject, qt_meta_stringdata_ConningKongsberUDPThread.data,
      qt_meta_data_ConningKongsberUDPThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ConningKongsberUDPThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConningKongsberUDPThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ConningKongsberUDPThread.stringdata0))
        return static_cast<void*>(const_cast< ConningKongsberUDPThread*>(this));
    return ConningKongsberGenericThread::qt_metacast(_clname);
}

int ConningKongsberUDPThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ConningKongsberGenericThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_ConningKongsberSerialThread_t {
    QByteArrayData data[3];
    char stringdata0[34];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConningKongsberSerialThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConningKongsberSerialThread_t qt_meta_stringdata_ConningKongsberSerialThread = {
    {
QT_MOC_LITERAL(0, 0, 27), // "ConningKongsberSerialThread"
QT_MOC_LITERAL(1, 28, 4), // "stop"
QT_MOC_LITERAL(2, 33, 0) // ""

    },
    "ConningKongsberSerialThread\0stop\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConningKongsberSerialThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void ConningKongsberSerialThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConningKongsberSerialThread *_t = static_cast<ConningKongsberSerialThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stop(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ConningKongsberSerialThread::staticMetaObject = {
    { &ConningKongsberGenericThread::staticMetaObject, qt_meta_stringdata_ConningKongsberSerialThread.data,
      qt_meta_data_ConningKongsberSerialThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ConningKongsberSerialThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConningKongsberSerialThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ConningKongsberSerialThread.stringdata0))
        return static_cast<void*>(const_cast< ConningKongsberSerialThread*>(this));
    return ConningKongsberGenericThread::qt_metacast(_clname);
}

int ConningKongsberSerialThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ConningKongsberGenericThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_ConningKongsberTestThread_t {
    QByteArrayData data[3];
    char stringdata0[32];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConningKongsberTestThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConningKongsberTestThread_t qt_meta_stringdata_ConningKongsberTestThread = {
    {
QT_MOC_LITERAL(0, 0, 25), // "ConningKongsberTestThread"
QT_MOC_LITERAL(1, 26, 4), // "stop"
QT_MOC_LITERAL(2, 31, 0) // ""

    },
    "ConningKongsberTestThread\0stop\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConningKongsberTestThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void ConningKongsberTestThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConningKongsberTestThread *_t = static_cast<ConningKongsberTestThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stop(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ConningKongsberTestThread::staticMetaObject = {
    { &ConningKongsberGenericThread::staticMetaObject, qt_meta_stringdata_ConningKongsberTestThread.data,
      qt_meta_data_ConningKongsberTestThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ConningKongsberTestThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConningKongsberTestThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ConningKongsberTestThread.stringdata0))
        return static_cast<void*>(const_cast< ConningKongsberTestThread*>(this));
    return ConningKongsberGenericThread::qt_metacast(_clname);
}

int ConningKongsberTestThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ConningKongsberGenericThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
