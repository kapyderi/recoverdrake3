/****************************************************************************
** Meta object code from reading C++ file 'calculadora.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "calculadora.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'calculadora.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Calculadora[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      40,   12,   12,   12, 0x08,
      66,   12,   12,   12, 0x08,
      92,   12,   12,   12, 0x08,
     118,   12,   12,   12, 0x08,
     144,   12,   12,   12, 0x08,
     170,   12,   12,   12, 0x08,
     197,   12,   12,   12, 0x08,
     224,   12,   12,   12, 0x08,
     251,   12,   12,   12, 0x08,
     278,   12,   12,   12, 0x08,
     305,   12,   12,   12, 0x08,
     332,   12,   12,   12, 0x08,
     365,  359,   12,   12, 0x08,
     403,   12,   12,   12, 0x08,
     412,  359,   12,   12, 0x08,
     452,   12,   12,   12, 0x08,
     478,   12,   12,   12, 0x08,
     505,   12,   12,   12, 0x08,
     532,   12,   12,   12, 0x08,
     559,   12,   12,   12, 0x08,
     583,   12,   12,   12, 0x08,
     610,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Calculadora[] = {
    "Calculadora\0\0on_pushButton_28_clicked()\0"
    "on_pushButton_4_clicked()\0"
    "on_pushButton_5_clicked()\0"
    "on_pushButton_6_clicked()\0"
    "on_pushButton_8_clicked()\0"
    "on_pushButton_9_clicked()\0"
    "on_pushButton_10_clicked()\0"
    "on_pushButton_14_clicked()\0"
    "on_pushButton_15_clicked()\0"
    "on_pushButton_16_clicked()\0"
    "on_pushButton_20_clicked()\0"
    "on_pushButton_27_clicked()\0"
    "on_pushButton_21_clicked()\0value\0"
    "on_horizontalSlider_valueChanged(int)\0"
    "Cambio()\0on_horizontalSlider_2_valueChanged(int)\0"
    "on_pushButton_2_clicked()\0"
    "on_pushButton_30_clicked()\0"
    "on_pushButton_32_clicked()\0"
    "on_pushButton_33_clicked()\0"
    "on_pushButton_clicked()\0"
    "on_pushButton_11_clicked()\0"
    "on_pushButton_34_clicked()\0"
};

void Calculadora::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Calculadora *_t = static_cast<Calculadora *>(_o);
        switch (_id) {
        case 0: _t->on_pushButton_28_clicked(); break;
        case 1: _t->on_pushButton_4_clicked(); break;
        case 2: _t->on_pushButton_5_clicked(); break;
        case 3: _t->on_pushButton_6_clicked(); break;
        case 4: _t->on_pushButton_8_clicked(); break;
        case 5: _t->on_pushButton_9_clicked(); break;
        case 6: _t->on_pushButton_10_clicked(); break;
        case 7: _t->on_pushButton_14_clicked(); break;
        case 8: _t->on_pushButton_15_clicked(); break;
        case 9: _t->on_pushButton_16_clicked(); break;
        case 10: _t->on_pushButton_20_clicked(); break;
        case 11: _t->on_pushButton_27_clicked(); break;
        case 12: _t->on_pushButton_21_clicked(); break;
        case 13: _t->on_horizontalSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->Cambio(); break;
        case 15: _t->on_horizontalSlider_2_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->on_pushButton_2_clicked(); break;
        case 17: _t->on_pushButton_30_clicked(); break;
        case 18: _t->on_pushButton_32_clicked(); break;
        case 19: _t->on_pushButton_33_clicked(); break;
        case 20: _t->on_pushButton_clicked(); break;
        case 21: _t->on_pushButton_11_clicked(); break;
        case 22: _t->on_pushButton_34_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Calculadora::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Calculadora::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Calculadora,
      qt_meta_data_Calculadora, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Calculadora::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Calculadora::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Calculadora::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Calculadora))
        return static_cast<void*>(const_cast< Calculadora*>(this));
    return QDialog::qt_metacast(_clname);
}

int Calculadora::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    return _id;
}
QT_END_MOC_NAMESPACE