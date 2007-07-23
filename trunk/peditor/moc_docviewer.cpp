/****************************************************************************
** Meta object code from reading C++ file 'docviewer.h'
**
** Created: Fri Jul 20 17:14:06 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "docviewer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'docviewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_DocViewer[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_DocViewer[] = {
    "DocViewer\0"
};

const QMetaObject DocViewer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DocViewer,
      qt_meta_data_DocViewer, 0 }
};

const QMetaObject *DocViewer::metaObject() const
{
    return &staticMetaObject;
}

void *DocViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DocViewer))
	return static_cast<void*>(const_cast< DocViewer*>(this));
    return QWidget::qt_metacast(_clname);
}

int DocViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
