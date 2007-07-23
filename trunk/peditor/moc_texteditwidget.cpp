/****************************************************************************
** Meta object code from reading C++ file 'texteditwidget.h'
**
** Created: Fri Jul 20 17:14:00 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "texteditwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'texteditwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_NumberBar[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_NumberBar[] = {
    "NumberBar\0"
};

const QMetaObject NumberBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_NumberBar,
      qt_meta_data_NumberBar, 0 }
};

const QMetaObject *NumberBar::metaObject() const
{
    return &staticMetaObject;
}

void *NumberBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NumberBar))
	return static_cast<void*>(const_cast< NumberBar*>(this));
    return QWidget::qt_metacast(_clname);
}

int NumberBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_TextEditWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      22,   16,   15,   15, 0x05,
      46,   41,   15,   15, 0x05,
      75,   66,   15,   15, 0x05,
     114,  102,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     145,   15,   15,   15, 0x0a,
     166,   15,   15,   15, 0x0a,
     186,  179,   15,   15, 0x0a,
     221,  214,  209,   15, 0x0a,
     237,   15,   15,   15, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_TextEditWidget[] = {
    "TextEditWidget\0\0state\0highlighting(bool)\0"
    "word\0mouseHover(QString)\0pos,word\0"
    "mouseHover(QPoint,QString)\0line,column\0"
    "cursorPositionChanged(int,int)\0"
    "toggleHighlighting()\0updateFont()\0"
    "_state\0setLineNumbering(bool)\0bool\0"
    "lineNo\0moveToLine(int)\0cursorChanged()\0"
};

const QMetaObject TextEditWidget::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_TextEditWidget,
      qt_meta_data_TextEditWidget, 0 }
};

const QMetaObject *TextEditWidget::metaObject() const
{
    return &staticMetaObject;
}

void *TextEditWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TextEditWidget))
	return static_cast<void*>(const_cast< TextEditWidget*>(this));
    return QFrame::qt_metacast(_clname);
}

int TextEditWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: highlighting((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: mouseHover((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: mouseHover((*reinterpret_cast< const QPoint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: cursorPositionChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: toggleHighlighting(); break;
        case 5: updateFont(); break;
        case 6: setLineNumbering((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: { bool _r = moveToLine((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: cursorChanged(); break;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void TextEditWidget::highlighting(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TextEditWidget::mouseHover(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TextEditWidget::mouseHover(const QPoint & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TextEditWidget::cursorPositionChanged(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
