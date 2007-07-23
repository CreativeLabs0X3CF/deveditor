/****************************************************************************
** Meta object code from reading C++ file 'newprojectwidget.h'
**
** Created: Fri Jul 20 17:14:04 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "newprojectwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'newprojectwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_NewProjectWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      28,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      61,   17,   17,   17, 0x08,
      79,   17,   17,   17, 0x08,
      97,   17,   17,   17, 0x08,
     124,  118,   17,   17, 0x08,
     162,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_NewProjectWidget[] = {
    "NewProjectWidget\0\0dir,templ\0"
    "createProgramme(QString,QString)\0"
    "getLocationPath()\0locationChanged()\0"
    "regenerateProgName()\0index\0"
    "templateSelectionChanged(QModelIndex)\0"
    "createProg()\0"
};

const QMetaObject NewProjectWidget::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_NewProjectWidget,
      qt_meta_data_NewProjectWidget, 0 }
};

const QMetaObject *NewProjectWidget::metaObject() const
{
    return &staticMetaObject;
}

void *NewProjectWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NewProjectWidget))
	return static_cast<void*>(const_cast< NewProjectWidget*>(this));
    return QDialog::qt_metacast(_clname);
}

int NewProjectWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: createProgramme((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: getLocationPath(); break;
        case 2: locationChanged(); break;
        case 3: regenerateProgName(); break;
        case 4: templateSelectionChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 5: createProg(); break;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void NewProjectWidget::createProgramme(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
