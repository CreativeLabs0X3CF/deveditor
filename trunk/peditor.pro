win32 {
    SUBDIRS += assistant src
}

unix {
    SUBDIRS += src

    INSTALLS += peditor
    INSTALLS += cppreference

    peditor.files = ./bin/peditor
    cppreference.extra = ln -sf /usr/local/bin/peditor /usr/local/bin/cppreference

    peditor.path = /usr/local/bin
    cppreference.path = /usr/local/bin

}

TEMPLATE = subdirs

CONFIG += warn_on \
          qt \
          thread \
          ordered

Language = C++

DESTDIR = .

