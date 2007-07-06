win32 {
    SUBDIRS += assistant src
}

unix {
    SUBDIRS += src

    INSTALLS += peditor

    peditor.files += ./bin/peditor

    peditor.path = /usr/local/bin
}

TEMPLATE = subdirs

CONFIG += warn_on \
          qt \
          thread \
          debug \
 ordered

Language = C++

DESTDIR = .

