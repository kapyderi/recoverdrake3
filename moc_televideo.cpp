/****************************************************************************
** Meta object code from reading C++ file 'televideo.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "televideo.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'televideo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Televideo[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      37,   10,   10,   10, 0x08,
      63,   10,   10,   10, 0x08,
      89,   10,   10,   10, 0x08,
     120,  113,   10,   10, 0x08,
     144,   10,   10,   10, 0x08,
     159,   10,   10,   10, 0x08,
     166,   10,   10,   10, 0x08,
     192,   10,   10,   10, 0x08,
     218,   10,   10,   10, 0x08,
     244,   10,   10,   10, 0x08,
     270,   10,   10,   10, 0x08,
     296,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Televideo[] = {
    "Televideo\0\0on_pushButton_7_clicked()\0"
    "on_pushButton_5_clicked()\0"
    "on_pushButton_4_clicked()\0"
    "on_pushButton_clicked()\0actual\0"
    "cambiaFila(QModelIndex)\0tableClicked()\0"
    "play()\0on_pushButton_3_clicked()\0"
    "on_pushButton_6_clicked()\0"
    "on_pushButton_2_clicked()\0"
    "on_pushButton_8_clicked()\0"
    "on_pushButton_9_clicked()\0Comprobar()\0"
};

void Televideo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Televideo *_t = static_cast<Televideo *>(_o);
        switch (_id) {
        case 0: _t->on_pushButton_7_clicked(); break;
        case 1: _t->on_pushButton_5_clicked(); break;
        case 2: _t->on_pushButton_4_clicked(); break;
        case 3: _t->on_pushButton_clicked(); break;
        case 4: _t->cambiaFila((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 5: _t->tableClicked(); break;
        case 6: _t->play(); break;
        case 7: _t->on_pushButton_3_clicked(); break;
        case 8: _t->on_pushButton_6_clicked(); break;
        case 9: _t->on_pushButton_2_clicked(); break;
        case 10: _t->on_pushButton_8_clicked(); break;
        case 11: _t->on_pushButton_9_clicked(); break;
        case 12: _t->Comprobar(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Televideo::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Televideo::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Televideo,
      qt_meta_data_Televideo, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Televideo::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Televideo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Televideo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Televideo))
        return static_cast<void*>(const_cast< Televideo*>(this));
    return QDialog::qt_metacast(_clname);
}

int Televideo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE