SOURCES += deveditor.cpp \
           main.cpp \
 highlighter.cpp \
 texteditwidget.cpp \
 environment.cpp
HEADERS += deveditor.h \
 highlighter.h \
 texteditwidget.h \
 numberbar_interface.h \
 environment.h
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt
TARGET = ../bin/deveditor
RESOURCES = application.qrc
