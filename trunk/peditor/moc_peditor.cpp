/****************************************************************************
** Meta object code from reading C++ file 'peditor.h'
**
** Created: Fri Jul 20 17:14:03 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "peditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'peditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_TabWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      17,   11,   10,   10, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_TabWidget[] = {
    "TabWidget\0\0,_tab\0contextMenuAt(QPoint,int)\0"
};

const QMetaObject TabWidget::staticMetaObject = {
    { &QTabWidget::staticMetaObject, qt_meta_stringdata_TabWidget,
      qt_meta_data_TabWidget, 0 }
};

const QMetaObject *TabWidget::metaObject() const
{
    return &staticMetaObject;
}

void *TabWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TabWidget))
	return static_cast<void*>(const_cast< TabWidget*>(this));
    return QTabWidget::qt_metacast(_clname);
}

int TabWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTabWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: contextMenuAt((*reinterpret_cast< const QPoint(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void TabWidget::contextMenuAt(const QPoint & _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_PEditor[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      49,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,
      32,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      65,   56,   51,    8, 0x0a,
      87,    8,    8,    8, 0x08,
      97,   56,    8,    8, 0x08,
     111,    8,    8,    8, 0x28,
     118,    8,    8,    8, 0x08,
     128,    8,    8,    8, 0x08,
     141,  137,   51,    8, 0x08,
     151,    8,   51,    8, 0x28,
     158,  137,   51,    8, 0x08,
     170,    8,   51,    8, 0x28,
     179,    8,    8,    8, 0x08,
     195,  187,    8,    8, 0x08,
     221,    8,    8,    8, 0x08,
     243,    8,    8,    8, 0x08,
     270,    8,    8,    8, 0x08,
     297,  292,    8,    8, 0x08,
     314,    8,    8,    8, 0x08,
     332,  326,    8,    8, 0x08,
     368,    8,    8,    8, 0x08,
     381,    8,    8,    8, 0x08,
     395,    8,    8,    8, 0x08,
     420,  411,    8,    8, 0x08,
     439,    8,    8,    8, 0x08,
     459,  449,    8,    8, 0x08,
     492,    8,    8,    8, 0x08,
     503,    8,    8,    8, 0x08,
     520,    8,    8,    8, 0x08,
     532,    8,    8,    8, 0x08,
     546,    8,    8,    8, 0x08,
     559,    8,    8,    8, 0x08,
     576,    8,    8,    8, 0x08,
     592,    8,    8,    8, 0x08,
     621,  611,    8,    8, 0x08,
     639,    8,    8,    8, 0x28,
     653,    8,    8,    8, 0x08,
     666,    8,    8,    8, 0x08,
     682,    8,    8,    8, 0x08,
     698,    8,    8,    8, 0x08,
     708,    8,    8,    8, 0x08,
     722,    8,    8,    8, 0x08,
     739,  737,    8,    8, 0x08,
     773,    8,    8,    8, 0x08,
     785,    8,    8,    8, 0x08,
     801,    8,    8,    8, 0x08,
     816,    8,    8,    8, 0x08,
     840,    8,    8,    8, 0x08,
     858,  852,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PEditor[] = {
    "PEditor\0\0compilationSuccesful()\0"
    "linkingSuccesful()\0bool\0fileName\0"
    "switchToFile(QString)\0newFile()\0"
    "open(QString)\0open()\0saveAll()\0reload()\0"
    "tab\0save(int)\0save()\0saveAs(int)\0"
    "saveAs()\0about()\0changed\0"
    "documentWasModified(bool)\0"
    "documentWasModified()\0toggleSyntaxHighlighting()\0"
    "toggleLineNumbering()\0_tab\0switchToTab(int)\0"
    "removeTab()\0state\0setSyntaxHighlightingMenuItem(bool)\0"
    "textBigger()\0textSmaller()\0setFontFamily()\0"
    "line,col\0updatePos(int,int)\0newProg()\0"
    "dir,templ\0createProgramme(QString,QString)\0"
    "openProg()\0openRecentProg()\0closeProg()\0"
    "compileNext()\0compileAll()\0compileChanged()\0"
    "compileFailed()\0compileSuccesful()\0"
    "dontClear\0linkObjects(bool)\0linkObjects()\0"
    "linkFailed()\0linkSuccesful()\0"
    "continueBuild()\0runProg()\0continueRun()\0"
    "continueRun2()\0,\0runDone(int,QProcess::ExitStatus)\0"
    "buildProg()\0disableCLActs()\0enableCLActs()\0"
    "canCompileChanged(bool)\0assistant()\0"
    ",_tab\0showContextMenu(QPoint,int)\0"
};

const QMetaObject PEditor::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_PEditor,
      qt_meta_data_PEditor, 0 }
};

const QMetaObject *PEditor::metaObject() const
{
    return &staticMetaObject;
}

void *PEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PEditor))
	return static_cast<void*>(const_cast< PEditor*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int PEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: compilationSuccesful(); break;
        case 1: linkingSuccesful(); break;
        case 2: { bool _r = switchToFile((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: newFile(); break;
        case 4: open((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: open(); break;
        case 6: saveAll(); break;
        case 7: reload(); break;
        case 8: { bool _r = save((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 9: { bool _r = save();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: { bool _r = saveAs((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 11: { bool _r = saveAs();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 12: about(); break;
        case 13: documentWasModified((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: documentWasModified(); break;
        case 15: toggleSyntaxHighlighting(); break;
        case 16: toggleLineNumbering(); break;
        case 17: switchToTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: removeTab(); break;
        case 19: setSyntaxHighlightingMenuItem((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 20: textBigger(); break;
        case 21: textSmaller(); break;
        case 22: setFontFamily(); break;
        case 23: updatePos((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 24: newProg(); break;
        case 25: createProgramme((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 26: openProg(); break;
        case 27: openRecentProg(); break;
        case 28: closeProg(); break;
        case 29: compileNext(); break;
        case 30: compileAll(); break;
        case 31: compileChanged(); break;
        case 32: compileFailed(); break;
        case 33: compileSuccesful(); break;
        case 34: linkObjects((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 35: linkObjects(); break;
        case 36: linkFailed(); break;
        case 37: linkSuccesful(); break;
        case 38: continueBuild(); break;
        case 39: runProg(); break;
        case 40: continueRun(); break;
        case 41: continueRun2(); break;
        case 42: runDone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 43: buildProg(); break;
        case 44: disableCLActs(); break;
        case 45: enableCLActs(); break;
        case 46: canCompileChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 47: assistant(); break;
        case 48: showContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        }
        _id -= 49;
    }
    return _id;
}

// SIGNAL 0
void PEditor::compilationSuccesful()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void PEditor::linkingSuccesful()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
