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

TARGET = peditor

RESOURCES = application.qrc

CONFIG -= stl \
          thread

DESTDIR = ../bin/

QT += xml

QMAKE_CXXFLAGS_DEBUG += -Wall

win32 {
    INCLUDEPATH += ../

    LIBS += ../bin/libassistant.a

    TARGETDEPS += ../bin/libassistant.a
}

unix {
    QMAKE_POST_LINK = /bin/ln -sf $(DESTDIR)/peditor $(DESTDIR)/cppreference

    QMAKE_CLEAN += ../bin/cppreference
}
