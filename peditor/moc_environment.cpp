/****************************************************************************
** Meta object code from reading C++ file 'environment.h'
**
** Created: Fri Jul 20 17:14:01 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "environment.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'environment.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_EnvironmentConfigurationWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      32,   31,   31,   31, 0x05,
      51,   31,   31,   31, 0x05,
      74,   71,   31,   31, 0x05,

 // slots: signature, parameters, type, tag, flags
     119,   31,   31,   31, 0x08,
     131,   31,   31,   31, 0x08,
     144,   31,   31,   31, 0x08,
     151,   31,   31,   31, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_EnvironmentConfigurationWidget[] = {
    "EnvironmentConfigurationWidget\0\0"
    "ccChanged(QString)\0cppChanged(QString)\0"
    ",,\0optsChanged(QString,QStringList,QStringList)\0"
    "getCcPath()\0getCppPath()\0test()\0"
    "updateOpts()\0"
};

const QMetaObject EnvironmentConfigurationWidget::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_EnvironmentConfigurationWidget,
      qt_meta_data_EnvironmentConfigurationWidget, 0 }
};

const QMetaObject *EnvironmentConfigurationWidget::metaObject() const
{
    return &staticMetaObject;
}

void *EnvironmentConfigurationWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EnvironmentConfigurationWidget))
	return static_cast<void*>(const_cast< EnvironmentConfigurationWidget*>(this));
    return QDialog::qt_metacast(_clname);
}

int EnvironmentConfigurationWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ccChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: cppChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: optsChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2])),(*reinterpret_cast< const QStringList(*)>(_a[3]))); break;
        case 3: getCcPath(); break;
        case 4: getCppPath(); break;
        case 5: test(); break;
        case 6: updateOpts(); break;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void EnvironmentConfigurationWidget::ccChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void EnvironmentConfigurationWidget::cppChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void EnvironmentConfigurationWidget::optsChanged(const QString & _t1, const QStringList & _t2, const QStringList & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
static const uint qt_meta_data_Environment[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      32,   12,   12,   12, 0x05,
      48,   12,   12,   12, 0x05,
      64,   12,   12,   12, 0x05,
      79,   77,   12,   12, 0x05,
     113,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     140,  137,   12,   12, 0x0a,
     186,  181,   12,   12, 0x0a,
     201,  181,   12,   12, 0x0a,
     237,  217,   12,   12, 0x0a,
     284,  217,   12,   12, 0x0a,
     328,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Environment[] = {
    "Environment\0\0compileSuccesful()\0"
    "compileFailed()\0linkSuccesful()\0"
    "linkFailed()\0,\0runDone(int,QProcess::ExitStatus)\0"
    "canCompileChanged(bool)\0,,\0"
    "setOpts(QString,QStringList,QStringList)\0"
    "path\0setCc(QString)\0setCpp(QString)\0"
    "exitCode,exitStatus\0"
    "compileProcessExited(int,QProcess::ExitStatus)\0"
    "linkProcessExited(int,QProcess::ExitStatus)\0"
    "compilationFailed()\0"
};

const QMetaObject Environment::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Environment,
      qt_meta_data_Environment, 0 }
};

const QMetaObject *Environment::metaObject() const
{
    return &staticMetaObject;
}

void *Environment::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Environment))
	return static_cast<void*>(const_cast< Environment*>(this));
    return QObject::qt_metacast(_clname);
}

int Environment::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: compileSuccesful(); break;
        case 1: compileFailed(); break;
        case 2: linkSuccesful(); break;
        case 3: linkFailed(); break;
        case 4: runDone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 5: canCompileChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: setOpts((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2])),(*reinterpret_cast< const QStringList(*)>(_a[3]))); break;
        case 7: setCc((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: setCpp((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: compileProcessExited((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 10: linkProcessExited((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 11: compilationFailed(); break;
        }
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void Environment::compileSuccesful()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Environment::compileFailed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Environment::linkSuccesful()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Environment::linkFailed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void Environment::runDone(int _t1, QProcess::ExitStatus _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Environment::canCompileChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
