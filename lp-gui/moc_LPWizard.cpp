/****************************************************************************
** Meta object code from reading C++ file 'LPWizard.h'
**
** Created: Thu Oct 10 12:35:30 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "LPWizard.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LPWizard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LPWizard[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      25,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_LPWizard[] = {
    "LPWizard\0\0slotFinished()\0slotCancelled()\0"
};

void LPWizard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LPWizard *_t = static_cast<LPWizard *>(_o);
        switch (_id) {
        case 0: _t->slotFinished(); break;
        case 1: _t->slotCancelled(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData LPWizard::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LPWizard::staticMetaObject = {
    { &QWizard::staticMetaObject, qt_meta_stringdata_LPWizard,
      qt_meta_data_LPWizard, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LPWizard::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LPWizard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LPWizard::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LPWizard))
        return static_cast<void*>(const_cast< LPWizard*>(this));
    return QWizard::qt_metacast(_clname);
}

int LPWizard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizard::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
