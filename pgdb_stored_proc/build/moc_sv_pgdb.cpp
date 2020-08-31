/****************************************************************************
** Meta object code from reading C++ file 'sv_pgdb.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../svlib/sv_pgdb.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sv_pgdb.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SvPGDB_t {
    QByteArrayData data[15];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SvPGDB_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SvPGDB_t qt_meta_stringdata_SvPGDB = {
    {
QT_MOC_LITERAL(0, 0, 6), // "SvPGDB"
QT_MOC_LITERAL(1, 7, 7), // "execSQL"
QT_MOC_LITERAL(2, 15, 9), // "QSqlError"
QT_MOC_LITERAL(3, 25, 0), // ""
QT_MOC_LITERAL(4, 26, 9), // "queryText"
QT_MOC_LITERAL(5, 36, 11), // "QByteArray*"
QT_MOC_LITERAL(6, 48, 5), // "data1"
QT_MOC_LITERAL(7, 54, 5), // "data2"
QT_MOC_LITERAL(8, 60, 5), // "data3"
QT_MOC_LITERAL(9, 66, 10), // "QSqlQuery*"
QT_MOC_LITERAL(10, 77, 5), // "query"
QT_MOC_LITERAL(11, 83, 15), // "QSqlQueryModel*"
QT_MOC_LITERAL(12, 99, 5), // "model"
QT_MOC_LITERAL(13, 105, 16), // "checkTableExists"
QT_MOC_LITERAL(14, 122, 9) // "tableName"

    },
    "SvPGDB\0execSQL\0QSqlError\0\0queryText\0"
    "QByteArray*\0data1\0data2\0data3\0QSqlQuery*\0"
    "query\0QSqlQueryModel*\0model\0"
    "checkTableExists\0tableName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SvPGDB[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    3, 0x0a /* Public */,
       1,    5,   57,    3, 0x0a /* Public */,
       1,    4,   68,    3, 0x2a /* Public | MethodCloned */,
       1,    3,   77,    3, 0x2a /* Public | MethodCloned */,
       1,    2,   84,    3, 0x2a /* Public | MethodCloned */,
       1,    2,   89,    3, 0x0a /* Public */,
       1,    2,   94,    3, 0x0a /* Public */,
      13,    1,   99,    3, 0x0a /* Public */,

 // slots: parameters
    0x80000000 | 2, QMetaType::QString,    4,
    0x80000000 | 2, QMetaType::QString, 0x80000000 | 5, 0x80000000 | 5, 0x80000000 | 5, 0x80000000 | 9,    4,    6,    7,    8,   10,
    0x80000000 | 2, QMetaType::QString, 0x80000000 | 5, 0x80000000 | 5, 0x80000000 | 5,    4,    6,    7,    8,
    0x80000000 | 2, QMetaType::QString, 0x80000000 | 5, 0x80000000 | 5,    4,    6,    7,
    0x80000000 | 2, QMetaType::QString, 0x80000000 | 5,    4,    6,
    0x80000000 | 2, QMetaType::QString, 0x80000000 | 9,    4,   10,
    0x80000000 | 2, QMetaType::QString, 0x80000000 | 11,    4,   12,
    QMetaType::Bool, QMetaType::QString,   14,

       0        // eod
};

void SvPGDB::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SvPGDB *_t = static_cast<SvPGDB *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { QSqlError _r = _t->execSQL((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QSqlError*>(_a[0]) = _r; }  break;
        case 1: { QSqlError _r = _t->execSQL((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QByteArray*(*)>(_a[2])),(*reinterpret_cast< QByteArray*(*)>(_a[3])),(*reinterpret_cast< QByteArray*(*)>(_a[4])),(*reinterpret_cast< QSqlQuery*(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< QSqlError*>(_a[0]) = _r; }  break;
        case 2: { QSqlError _r = _t->execSQL((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QByteArray*(*)>(_a[2])),(*reinterpret_cast< QByteArray*(*)>(_a[3])),(*reinterpret_cast< QByteArray*(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QSqlError*>(_a[0]) = _r; }  break;
        case 3: { QSqlError _r = _t->execSQL((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QByteArray*(*)>(_a[2])),(*reinterpret_cast< QByteArray*(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QSqlError*>(_a[0]) = _r; }  break;
        case 4: { QSqlError _r = _t->execSQL((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QByteArray*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QSqlError*>(_a[0]) = _r; }  break;
        case 5: { QSqlError _r = _t->execSQL((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QSqlQuery*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QSqlError*>(_a[0]) = _r; }  break;
        case 6: { QSqlError _r = _t->execSQL((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QSqlQueryModel*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QSqlError*>(_a[0]) = _r; }  break;
        case 7: { bool _r = _t->checkTableExists((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSqlQueryModel* >(); break;
            }
            break;
        }
    }
}

const QMetaObject SvPGDB::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SvPGDB.data,
      qt_meta_data_SvPGDB,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SvPGDB::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SvPGDB::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SvPGDB.stringdata0))
        return static_cast<void*>(const_cast< SvPGDB*>(this));
    return QObject::qt_metacast(_clname);
}

int SvPGDB::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
