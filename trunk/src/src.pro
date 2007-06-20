SOURCES += deveditor.cpp \
           main.cpp \
 highlighter.cpp \
 texteditwidget.cpp \
 environment.cpp \
 messagebox.cpp
HEADERS += deveditor.h \
 highlighter.h \
 texteditwidget.h \
 environment.h \
 messagebox.h
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt
TARGET = ../bin/deveditor
RESOURCES = application.qrc
