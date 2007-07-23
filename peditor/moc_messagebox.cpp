/****************************************************************************
** Meta object code from reading C++ file 'messagebox.h'
**
** Created: Fri Jul 20 17:14:02 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "messagebox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'messagebox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_MessageBox[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      21,   12,   11,   11, 0x05,
      43,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      63,   59,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MessageBox[] = {
    "MessageBox\0\0fileName\0switchToFile(QString)\0"
    "moveToLine(int)\0url\0moveTo(QUrl)\0"
};

const QMetaObject MessageBox::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MessageBox,
      qt_meta_data_MessageBox, 0 }
};

const QMetaObject *MessageBox::metaObject() const
{
    return &staticMetaObject;
}

void *MessageBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MessageBox))
	return static_cast<void*>(const_cast< MessageBox*>(this));
    return QWidget::qt_metacast(_clname);
}

int MessageBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: switchToFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: moveToLine((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: moveTo((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void MessageBox::switchToFile(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MessageBox::moveToLine(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
