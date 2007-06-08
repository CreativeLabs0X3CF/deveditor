SOURCES += deveditor.cpp \
           main.cpp \
 highlighter.cpp \
 texteditwidget.cpp
HEADERS += deveditor.h \
 highlighter.h \
 texteditwidget.h
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt
TARGET = ../bin/deveditor
RESOURCES = application.qrc
