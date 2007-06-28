SOURCES += main.cpp \
 highlighter.cpp \
 texteditwidget.cpp \
 environment.cpp \
 messagebox.cpp \
 peditor.cpp
HEADERS += highlighter.h \
 texteditwidget.h \
 environment.h \
 messagebox.h \
 peditor.h
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt \
 release
TARGET = ../bin/peditor
RESOURCES = application.qrc
CONFIG -= debug \
stl
DESTDIR = .

INSTALLS += peditor


peditor.files += ../bin/peditor

peditor.path = /usr/local/bin

