SOURCES += main.cpp \
 highlighter.cpp \
 texteditwidget.cpp \
 environment.cpp \
 messagebox.cpp \
 peditor.cpp \
 newprojectwidget.cpp
HEADERS += highlighter.h \
 texteditwidget.h \
 environment.h \
 messagebox.h \
 peditor.h \
 newprojectwidget.h
TEMPLATE = app
CONFIG += warn_on \
	  qt \
 release
TARGET = ../bin/peditor
RESOURCES = application.qrc
CONFIG -= debug \
stl \
 thread
DESTDIR = .

INSTALLS += peditor


peditor.files += ../bin/peditor

peditor.path = /usr/local/bin


