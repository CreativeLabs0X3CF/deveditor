SOURCES += main.cpp \
 highlighter.cpp \
 texteditwidget.cpp \
 environment.cpp \
 messagebox.cpp \
 peditor.cpp \
 newprojectwidget.cpp \
 aboutwidget.cpp \
 docviewer.cpp

HEADERS += highlighter.h \
 texteditwidget.h \
 environment.h \
 messagebox.h \
 peditor.h \
 newprojectwidget.h \
 aboutwidget.h \
 docviewer.h

TEMPLATE = app

CONFIG += warn_on \
	  qt \
 assistant \
 x11

TARGET = ../bin/peditor

RESOURCES = application.qrc

CONFIG -= stl \
 thread

DESTDIR = .

QT += xml

QMAKE_CXXFLAGS_DEBUG += -Wall

win32 {
    INCLUDEPATH += ../assistant

    LIBS += ../bin/libassistant.a

    TARGETDEPS += ../bin/libassistant.a
}
