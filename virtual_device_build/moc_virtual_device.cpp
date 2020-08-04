/****************************************************************************
** Meta object code from reading C++ file 'virtual_device.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../virtual_device/virtual_device.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'virtual_device.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VirtualDevice_t {
    QByteArrayData data[4];
    char stringdata0[39];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VirtualDevice_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VirtualDevice_t qt_meta_stringdata_VirtualDevice = {
    {
QT_MOC_LITERAL(0, 0, 13), // "VirtualDevice"
QT_MOC_LITERAL(1, 14, 10), // "stopThread"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 12) // "deleteThread"

    },
    "VirtualDevice\0stopThread\0\0deleteThread"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VirtualDevice[] = {

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

void VirtualDevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VirtualDevice *_t = static_cast<VirtualDevice *>(_o);
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
            typedef void (VirtualDevice::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VirtualDevice::stopThread)) {
                *result = 0;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject VirtualDevice::staticMetaObject = {
    { &ad::SvAbstractDevice::staticMetaObject, qt_meta_stringdata_VirtualDevice.data,
      qt_meta_data_VirtualDevice,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VirtualDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VirtualDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VirtualDevice.stringdata0))
        return static_cast<void*>(const_cast< VirtualDevice*>(this));
    return ad::SvAbstractDevice::qt_metacast(_clname);
}

int VirtualDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void VirtualDevice::stopThread()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_VirtualDeviceGenericThread_t {
    QByteArrayData data[1];
    char stringdata0[27];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VirtualDeviceGenericThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VirtualDeviceGenericThread_t qt_meta_stringdata_VirtualDeviceGenericThread = {
    {
QT_MOC_LITERAL(0, 0, 26) // "VirtualDeviceGenericThread"

    },
    "VirtualDeviceGenericThread"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VirtualDeviceGenericThread[] = {

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

void VirtualDeviceGenericThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject VirtualDeviceGenericThread::staticMetaObject = {
    { &ad::SvAbstractDeviceThread::staticMetaObject, qt_meta_stringdata_VirtualDeviceGenericThread.data,
      qt_meta_data_VirtualDeviceGenericThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VirtualDeviceGenericThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VirtualDeviceGenericThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VirtualDeviceGenericThread.stringdata0))
        return static_cast<void*>(const_cast< VirtualDeviceGenericThread*>(this));
    return ad::SvAbstractDeviceThread::qt_metacast(_clname);
}

int VirtualDeviceGenericThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ad::SvAbstractDeviceThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_VirtualDeviceUDPThread_t {
    QByteArrayData data[3];
    char stringdata0[29];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VirtualDeviceUDPThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VirtualDeviceUDPThread_t qt_meta_stringdata_VirtualDeviceUDPThread = {
    {
QT_MOC_LITERAL(0, 0, 22), // "VirtualDeviceUDPThread"
QT_MOC_LITERAL(1, 23, 4), // "stop"
QT_MOC_LITERAL(2, 28, 0) // ""

    },
    "VirtualDeviceUDPThread\0stop\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VirtualDeviceUDPThread[] = {

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

void VirtualDeviceUDPThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VirtualDeviceUDPThread *_t = static_cast<VirtualDeviceUDPThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stop(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject VirtualDeviceUDPThread::staticMetaObject = {
    { &VirtualDeviceGenericThread::staticMetaObject, qt_meta_stringdata_VirtualDeviceUDPThread.data,
      qt_meta_data_VirtualDeviceUDPThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VirtualDeviceUDPThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VirtualDeviceUDPThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VirtualDeviceUDPThread.stringdata0))
        return static_cast<void*>(const_cast< VirtualDeviceUDPThread*>(this));
    return VirtualDeviceGenericThread::qt_metacast(_clname);
}

int VirtualDeviceUDPThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VirtualDeviceGenericThread::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_VirtualDeviceSerialThread_t {
    QByteArrayData data[3];
    char stringdata0[32];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VirtualDeviceSerialThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VirtualDeviceSerialThread_t qt_meta_stringdata_VirtualDeviceSerialThread = {
    {
QT_MOC_LITERAL(0, 0, 25), // "VirtualDeviceSerialThread"
QT_MOC_LITERAL(1, 26, 4), // "stop"
QT_MOC_LITERAL(2, 31, 0) // ""

    },
    "VirtualDeviceSerialThread\0stop\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VirtualDeviceSerialThread[] = {

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

void VirtualDeviceSerialThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VirtualDeviceSerialThread *_t = static_cast<VirtualDeviceSerialThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stop(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject VirtualDeviceSerialThread::staticMetaObject = {
    { &VirtualDeviceGenericThread::staticMetaObject, qt_meta_stringdata_VirtualDeviceSerialThread.data,
      qt_meta_data_VirtualDeviceSerialThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VirtualDeviceSerialThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VirtualDeviceSerialThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VirtualDeviceSerialThread.stringdata0))
        return static_cast<void*>(const_cast< VirtualDeviceSerialThread*>(this));
    return VirtualDeviceGenericThread::qt_metacast(_clname);
}

int VirtualDeviceSerialThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VirtualDeviceGenericThread::qt_metacall(_c, _id, _a);
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
