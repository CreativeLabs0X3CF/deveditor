SOURCES += deveditor.cpp \
           main.cpp \
 highlighter.cpp
HEADERS += deveditor.h \
 highlighter.h
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt
TARGET = ../bin/deveditor
RESOURCES = application.qrc
